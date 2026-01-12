# Como começar o projeto minishell (42) — Guia para dupla

Este documento descreve um plano prático para começar o projeto `minishell` em dupla: divisão de responsabilidades, etapas de implementação e um checklist com todos os requisitos do enunciado. Use-o como guia vivo: atualize as tarefas conforme avançam.

## Contrato mínimo (inputs/outputs)
- Input: comandos digitados pelo usuário (linha de comando), variáveis de ambiente, arquivos no disco.
- Output: execução de comandos, alterações de diretório, manipulação de arquivos e códigos de saída corretos.
- Erros: mensagens claras em stderr e códigos de saída compatíveis com bash em casos comuns.

## Regras importantes do enunciado (resumo)
- O shell deve exibir um prompt, ter histórico (readline) e executar binários pesquisando no `PATH` ou por caminho relativo/absoluto.
- Permitido usar `libft` e as funções externas listadas no enunciado (readline, execve, fork, pipe, dup2, open, close, ...). Consulte o enunciado para a lista completa.
- Só é permitida no máximo uma variável global que armazene apenas o número do sinal recebido.
- Implementar redirecionamentos `<`, `>`, `>>`, `<<` (heredoc), pipes `|`, expansão de variáveis `$`, expansão de `$?` e builtins: `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
- Tratar corretamente aspas simples e duplas e não acrescentar comportamentos além do pedido (não implementar `;`, escapes complexos, etc.).

## Estrutura de trabalho sugerida
Crie (se quiser) a seguinte organização dentro do repositório:

- `src/` — código-fonte C (separe por módulos: lexer, parser, executor, builtins, utils)
- `include/` — cabeçalhos públicos (`minishell.h`, `lexer.h`, `parser.h`, ...)
- `tests/` — scripts de teste e casos manuais
- `docs/` — notas e design decisions (opcional)

Observação: você já tem `libft/` com utilitários; aproveite. O Makefile do projeto final deve compilar os `.c` e linkar com `libft` se necessário.

## Como dividir o trabalho entre a dupla
A ideia é separar responsabilidades por módulos independentes e usar integração contínua manual entre PRs/branches. Sugestão de divisão em fases curtas (sprints de 1–3 dias):

- Pessoa A (Responsável por Lexer/Parser + Expansão)
  - Implementa tokenização: separar palavras, tratar espaços, pipes, redirecionamentos e metacaracteres.
  - Implementa tratamento de aspas (' e ") e remoção das aspas nos tokens finais.
  - Implementa expansão de variáveis (`$VAR` e `$?`) e comportamento do heredoc (`<<`) no nível do parser/expansor.

- Pessoa B (Responsável por Executor + Builtins + Redirecionamentos)
  - Implementa execução simples: executar um comando sem pipes/redirecionamentos usando `fork/execve` e `waitpid`.
  - Implementa pipes (criar encadeamento de `pipe` + `fork` e `dup2`).
  - Implementa redirecionamentos (`<`, `>`, `>>`) manipulando `open`, `dup2`, `close`.
  - Implementa builtins (em processos pai quando necessário): `cd`, `exit`, `export`, `unset`, `env`, `pwd`, `echo -n`.

Colaboração e integração:
- Comecem cada feature em uma branch pequena. A pessoa A cria a API/estruturas de tokens e AST e um conjunto de testes simples; a pessoa B escreve código que consome essa API.
- Façam code reviews rápidos (antes de merge) e testem o binário resultante manualmente com casos simples.

Assumptions (1–2 razoáveis)
- Vamos manter estruturas de dados simples (lista ligada para tokens e comandos) e evitar singletons globais exceto pela variável global do sinal.
- O Makefile inicial pode compilar tudo sem otimizações estranhas; iremos polir depois.

## Checklist detalhado (estado atual)

Infra e build
- [x] Makefile presente com alvos: `NAME` (opcional), `all`, `clean`, `fclean`, `re`.
- [x] Projeto compila sem warnings tratados (compilado com -Wall -Wextra -Werror sem avisos no estado atual).

Funcionalidade básica
- [x] Prompt interativo exibido e input lido com `readline`. (implementado em `src/main.c`)
- [x] Histórico funcional (`add_history`, `rl_on_new_line` etc.). Nota: heredoc não deve adicionar entrada ao histórico. (implementado — adiciona apenas linhas não vazias)
- [x] Tratamento de EOF (Ctrl-D) para sair do shell. (implementado — `readline` retorna NULL)
- [x] Tratamento de Ctrl-C em modo interativo: mostra novo prompt na linha seguinte. (implementado — handler usa `rl_on_new_line`/`rl_replace_line`/`rl_redisplay`)
- [x] Ctrl-\ (SIGQUIT) não faz nada em modo interativo. (implementado — handler vazio)
- [x] Implementar apenas uma variável global para sinal (apenas número do sinal). (implementado: `g_last_signal` como `sig_atomic_t`)

Parsing e expansão
- [x] Tokenização básica implementada (palavras, pipes `|`, redirecionadores `<`, `>`, `>>`, `<<`) — `lexer_tokenize` em `src/lexer.c` junto com helpers em `src/lexer_helpers.c`, com `TOK_END` sentinel adicionado.
- [x] Tratamento de aspas (remoção das aspas no token final) implementado: `collect_word` constrói tokens sem as aspas.
 - [x] Tratamento completo de aspas duplas (expansão dentro de `"..."`) — lexer cria segmentos e `src/expander.c` faz expansão nos segmentos duplamente citados.
 - [x] Expansão de variáveis `$VAR` e `$?` usando o ambiente e último status — implementado (`src/expander.c`, `src/expander_env.c`, `src/expander_utils.c`).
 - [x] Heredoc (`<<`) leitura e armazenamento (arquivo temporário) implementados: o parser lê as linhas do heredoc usando `readline("heredoc> ")`, cria um arquivo temporário com `mkstemp()` e escreve o conteúdo nele; o caminho do tmpfile é guardado em um `t_redir` com tipo `HEREDOC`. A expansão de variáveis dentro do heredoc é aplicada apenas se o delimitador não estiver citado (implementado em `src/parser_heredoc.c`, `src/parser_heredoc_utils.c`, `src/parser_heredoc_tmp.c`).

Parser (tokens -> comandos)
- [x] Parser completo implementado (`src/parser.c`, `src/parser_cmd.c`, `src/parser_helpers.c`, `src/parser_redir.c`, `src/parser_utils.c`, `include/parser.h`): agrupa `TOK_WORD` em `argv`, associa redirecionamentos ao comando correto, separa comandos por `TOK_PIPE`, e processa até encontrar `TOK_END`.

Execução
- [x] Executar comandos por caminho absoluto/relativo ou pesquisando em `PATH` (implementado em `src/exec/find_path.c` com busca no PATH e verificação de acesso).
- [x] Redirecionamentos de I/O com `open`, `dup2`, `close`:
  - [x] `<` entrada (implementado em `src/exec/open_file.c` e `src/exec/setup_child.c`)
  - [x] `>` saída (truncate) (implementado em `src/exec/open_file.c` e `src/exec/setup_child.c`)
  - [x] `>>` saída (append) (implementado em `src/exec/open_file.c` e `src/exec/setup_child.c`)
  - [x] `<<` heredoc (suportado via arquivo temporário criado pelo parser)
- [x] Pipes (`|`) que conectam n comandos encadeados (implementado em `src/exec/add_pipe.c` e `src/exec/exe_cmd.c` usando fork + pipe + dup2).
- [x] Fechar descritores corretos e evitar leaks entre processos (implementado em `src/exec/close_pipe.c` e `src/exec/setup_child.c`).

Builtins (comportamento mínimo)
- [x] `echo` com suporte a `-n` (apagar newline quando `-n`) — implementado em `src/exec/builtin/ft_echo.c`.
- [x] `cd` com suporte a caminhos relativos/absolutos, `cd ~`, `cd` (HOME), `cd -` (OLDPWD). Atualiza `PWD`/`OLDPWD` corretamente — implementado em `src/exec/builtin/ft_cd.c`. **Executa no processo pai** para afetar o shell.
- [x] `pwd` sem opções imprime diretório atual — implementado em `src/exec/builtin/ft_pwd.c`.
- [x] `export` sem opções lista variáveis (formato `declare -x`), com argumentos adiciona/atualiza variáveis de ambiente. Validação de nomes (alfanumérico + underscore) — implementado em `src/exec/builtin/ft_export.c`. **Executa no processo pai** e modifica o ambiente global.
- [x] `unset` sem opções não faz nada (retorna sucesso), com argumentos remove variáveis de ambiente. Validação de nomes (alfanumérico + underscore) — implementado em `src/exec/builtin/ft_unset.c`. **Executa no processo pai** e modifica o ambiente global.
- [x] `env` sem argumentos imprime variáveis de ambiente — implementado em `src/exec/builtin/ft_env.c`.
- [x] `exit` sem opções sai do shell com o código apropriado — implementado em `src/exec/builtin/ft_exit.c` (verifica se comando é exit e encerra).

Sinais
 - [x] Implementar tratamento de SIGINT (Ctrl-C) e SIGQUIT (Ctrl-\) com a única variável global. (implementado em `src/main.c`)
 - [x] Handlers instalados com `sigaction()` em `src/main.c`. (implementado)
 - [ ] Em execução de pipeline/processos filhos, sinais devem ser repassados corretamente e comportamento similar ao bash.

Observação: o repasse correto de sinais para processos filhos em pipelines ainda não foi implementado; isso exige configurar grupos de processos e usar `setpgid`/`tcsetpgrp` conforme necessário.

Memória e qualidade
- [x] `free(line)` após uso (implementado em `src/main.c`)
- [x] Sistema de free completo implementado (`src/free_all_variables.c` com `free_all_commands`, `free_redirs`, liberação de env, path, pids)
- [x] Não utilizar globals além da variável de sinal (apenas `g_last_signal` é usado).

Testes e validação
- [x] Testes manuais básicos funcionando:
  - [x] Execução de um comando simples (echo, ls, pwd funcionam)
  - [x] Pipes: `echo test | cat`, `ls | wc -l` funcionam
  - [x] Redirecionamentos: `echo hi > file`, `cat < file`, `echo x >> file` funcionam
  - [x] Heredoc básico (implementado com arquivo temporário)
  - [x] Variáveis `$VAR` e `$?` expandem corretamente
  - [x] Builtin `cd` completamente implementado (com ~, -, HOME, atualização de PWD/OLDPWD)
  - [x] Builtins `export` e `unset` implementados (ver `src/exec/builtin/ft_export.c`, `src/exec/builtin/ft_unset.c`)

## Exemplo de plano em sprints (curto prazo)
- Sprint 1 (2 dias): ambiente, Makefile, prompt e readline, histórico.
- Sprint 2 (3 dias): lexer/tokenizer, aspas e expansão de variáveis.
- Sprint 3 (3 dias): executor básico, execve, PATH lookup.
- Sprint 4 (3 dias): pipes e redirecionamentos.
- Sprint 5 (2 dias): builtins e sinalização.
- Sprint 6 (2 dias): testes, validação, correções e preparação do submit.

## Boas práticas e dicas
- Testem frequentemente e fixem leaks cedo.
- Mantenham commits pequenos e descritivos. Comentem decisões importantes em `docs/`.
- Evitem otimizações prematuras: priorizem comportamento correto conforme enunciado.
- Reutilizem `libft` quando apropriado, mas verifiquem conformidade com funções permitidas.

## Recursos úteis
- Página do enunciado do minishell (leiam o texto completo e a lista de funções autorizadas).
- `man 2 fork`, `man 2 execve`, `man 2 pipe`, `man 3 readline`.

## Checklist final antes de entregar
- [ ] Passou em testes manuais básicos.
- [ ] Makefile e arquivos `.c`/`.h` prontos e seguindo as exigências.
- [ ] Ausência de usos globais indevidos (exceto sinal global).
- [ ] Comportamento dos builtins conforme enunciado.

---

Se quiser, eu posso:
- Gerar templates de arquivos (ex.: `src/lexer.c`, `include/minishell.h`, `Makefile` minimal) para começarem.
- Criar scripts de teste em `tests/` com casos básicos.

Marquem qual opção preferem e continuo com as próximas alterações.
