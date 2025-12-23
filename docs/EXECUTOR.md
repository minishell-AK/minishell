# Guia para o Executor (para o parceiro que implementa exec/pipes/redirs)

Este documento descreve a API que o parser fornece (`t_cmd`, `t_redir`) e dá exemplos práticos de como o executor deve:
- abrir e ligar redirecionamentos (input/output/append/heredoc),
- montar pipes entre processos,
- limpar arquivos temporários de heredoc,
- cooperar com o expander quanto ao `$?` (last status) e gerenciamento de memória.

Tudo abaixo está em português e com exemplos em C compatíveis com POSIX.

**Resumo das estruturas**
- `t_cmd` (definida em `include/parser.h`)
  - `char **args` — array NULL-terminated com o comando e argumentos prontos para `execve`.
  - `t_redir *redirs` — lista ligada contendo redirecionamentos do comando.
  - `t_cmd *next` — próximo comando após um `|` (pipe).

- `t_redir` (definida em `include/parser.h`)
  - `t_redir_type type` — enum: `REDIR_IN`, `REDIR_OUT`, `HEREDOC`, `APPEND`.
  - `char *file` — caminho do arquivo alvo; para `HEREDOC` é o caminho do tmpfile criado pelo parser (ex: `/tmp/minishell_heredoc_XXXXXX`).
  - `t_redir *next` — próximo redirecionamento do mesmo comando.

Propriedade da memória:
- O parser aloca `args` e `redir->file` (via `strdup`/`malloc`).
- A função `free_commands(t_cmd *cmds)` está disponível para liberar toda a lista após a execução.
- O executor NÃO precisa (nem deve) liberar essas strings enquanto usar os FDs; a entidade que chamou o parser (controlador principal) chamará `free_commands()` quando apropriado.

Responsabilidades do executor (alto nível)
- Criar processos (`fork`) e chamar `execve` no filho.
- Montar `pipe()` entre comandos conforme a lista `t_cmd` encadeada.
- Para cada comando, aplicar os `t_redir` correspondentes: abrir arquivos e `dup2()` para stdin/stdout conforme o tipo.
- Para `HEREDOC`, abrir o arquivo temporário gerado pelo parser com `O_RDONLY` como stdin.
- Gerenciar fechamento de descritores (fechar FDs duplicados, fechar extremidades de pipes não usadas).
- Após execução, remover (unlink) os arquivos temporários de heredoc (ou assim que for seguro), se desejar.
- Coletar `waitpid` e definir `last_status` (o valor que será passado ao expander para `$?`).

Exemplos práticos de código

1) Abrir um redirecionamento para um comando

```c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int apply_redirections(t_cmd *cmd)
{
    t_redir *r = cmd->redirs;
    int fd;

    while (r)
    {
        if (r->type == REDIR_IN)
        {
            fd = open(r->file, O_RDONLY);
            if (fd == -1) { perror(r->file); return -1; }
            if (dup2(fd, STDIN_FILENO) == -1) { perror("dup2"); close(fd); return -1; }
            close(fd);
        }
        else if (r->type == REDIR_OUT)
        {
            fd = open(r->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd == -1) { perror(r->file); return -1; }
            if (dup2(fd, STDOUT_FILENO) == -1) { perror("dup2"); close(fd); return -1; }
            close(fd);
        }
        else if (r->type == APPEND)
        {
            fd = open(r->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
            if (fd == -1) { perror(r->file); return -1; }
            if (dup2(fd, STDOUT_FILENO) == -1) { perror("dup2"); close(fd); return -1; }
            close(fd);
        }
        else if (r->type == HEREDOC)
        {
            /* Parser já criou o arquivo temporário e escreveu o conteúdo. Abrimos como leitura. */
            fd = open(r->file, O_RDONLY);
            if (fd == -1) { perror(r->file); return -1; }
            if (dup2(fd, STDIN_FILENO) == -1) { perror("dup2"); close(fd); return -1; }
            close(fd);
        }
        r = r->next;
    }
    return 0;
}
```

Notas sobre ordem de redirecionamentos:
- Se houver múltiplos redirecionamentos do mesmo tipo para um comando (ex.: `> a > b`), o comportamento típico é que o último redirecionamento prevalece — a ordem em que a lista `t_redir` foi preenchida pelo parser determina o resultado. Ajustem conforme política desejada.

2) Exemplo mínimo de execução de uma pipeline (esqueleto, sem tratamento de erros completo)

```c
/* Pseudocódigo C simplificado */

void execute_pipeline(t_cmd *head)
{
    int in_fd = STDIN_FILENO;
    t_cmd *cur = head;

    while (cur)
    {
        int pipefd[2] = {-1, -1};
        if (cur->next)
        {
            pipe(pipefd); // cria pipe para conexão com próximo comando
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            /* Child */
            if (in_fd != STDIN_FILENO)
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (pipefd[1] != -1)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            /* Aplicar redirecionamentos específicos do comando (inclui heredoc) */
            if (apply_redirections(cur) == -1)
                _exit(1);

            /* fechar file descriptors extras no child */
            if (pipefd[0] != -1) close(pipefd[0]);

            execve(cur->args[0], cur->args, /* ambiente */ NULL);
            perror("execve");
            _exit(127);
        }
        else
        {
            /* Parent */
            if (in_fd != STDIN_FILENO) close(in_fd);
            if (pipefd[1] != -1) close(pipefd[1]);
            /* o próximo in_fd é a leitura do pipe atual */
            in_fd = (pipefd[0] != -1) ? pipefd[0] : STDIN_FILENO;
            cur = cur->next;
        }
    }

    /* Parent deve esperar todos os filhos e coletar status */
}
```

3) Sobre limpeza (unlink) de arquivos temporários de heredoc

- O parser cria um tmpfile com `mkstemp(template)` e escreve o conteúdo. Ele **não** faz `unlink()` nem remove o arquivo porque o executor precisa abrir esse arquivo como stdin durante execução.
- Recomenda-se ao executor (ou ao controlador que coordena execução) remover (`unlink`) o arquivo temporário assim que for seguro fazê-lo. Duas opções:
  - Remover imediatamente após abrir o arquivo (em cada child que precisar dele): abrir, duplicar via `dup2(fd, STDIN_FILENO)`, `close(fd)`, `unlink(path)`. Isso garante que o arquivo será removido do namespace de diretório mas o descritor ainda manterá o conteúdo até fechado.
  - Remover após `waitpid`/exec de toda a pipeline: iterar pelos comandos e `unlink(redir->file)` para cada `HEREDOC` após a execução terminar.

Exemplo para remover após abrir (child):

```c
int fd = open(redir->file, O_RDONLY);
if (fd != -1)
{
    if (dup2(fd, STDIN_FILENO) == -1) { perror("dup2"); }
    close(fd);
    unlink(redir->file); /* remove do FS imediatamente */
}
```

Observação: remover imediatamente é seguro porque o descritor aberto mantém o conteúdo.

4) Propagação de `$?` (last status)

- O expander recebe `last_status` (ver `expand_tokens(t_token *head, char **envp, int last_status)`).
- Fluxo típico:
  1. Ler linha do usuário.
  2. `tokens = lexer_tokenize(line);`
  3. `expand_tokens(tokens, environ, last_status);`  // usa `last_status` do último comando
  4. `cmds = parse_tokens(tokens);`
  5. `exec` a pipeline e calcule o novo `last_status` (via `waitpid`/WEXITSTATUS/WIFSIGNALED` etc.).
  6. Repetir loop com `last_status` atualizado.

Assegurem que o executor retorne ao controlador (main loop) o valor correto de `last_status`.

5) Sinais e grupos de processos

- Para compatibilidade com bash, é comum:
  - Criar um novo process group para cada pipeline (usar `setpgid()`), colocar o grupo em foreground (`tcsetpgrp()`), e restaurar o terminal ao shell após execução.
  - Manipular sinais: o shell (pai) deve ignorar SIGINT/SIGQUIT enquanto espera; os filhos devem receber sinais normalmente.
- Isso é avançado e pode ser implementado depois do básico de fork/exec/pipe.

6) Testes sugeridos para o executor

- Teste básico: executar `/bin/echo` com argumentos.
- Pipes: `echo hi | tr a-z A-Z`.
- Redirecionamentos: `echo hi > tmp && cat < tmp`.
- Heredoc: `cat <<EOF\nhello\n$USER\nEOF` e garantir expansão conforme delimitador citado.
- Builtins: `cd` e `exit` exigem tratamento especial (alguns builtins devem ser executados no processo pai).

7) Exemplo mínimo de limpeza de heredocs após execução (parent)

```c
void cleanup_heredocs(t_cmd *head)
{
    t_cmd *c = head;
    while (c)
    {
        t_redir *r = c->redirs;
        while (r)
        {
            if (r->type == HEREDOC)
                unlink(r->file);
            r = r->next;
        }
        c = c->next;
    }
}
```

8) Observações finais

- Documentem na integração quem é responsável por `unlink()` e por `free_commands()` (recomendado: executor faz `unlink` dos heredocs, controlador chama `free_commands` depois que a execução estiver concluída).
- Testem com valgrind para detectar vazamentos nas rotinas novas.
- Para segurança, validem o prefixo dos arquivos temporários antes de `unlink()` (ex.: `strncmp(path, "/tmp/minishell_heredoc_", ...)`) para evitar apagar arquivos acidentalmente.

Se quiser, eu também:
- Escrevo um esqueleto de executor (`src/executor.c`) que implementa o básico (fork/exec, pipes simples e aplicação de redirecionamentos), pronto para o seu parceiro finalizar e ajustar para builtins/sinais.
- Adiciono testes automatizados simples em `tests/` para os casos comuns acima.
