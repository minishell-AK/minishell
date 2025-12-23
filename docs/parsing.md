# Parsing em Minishell

Este documento explica o que é parsing, qual é seu papel no projeto `minishell` e como implementá-lo passo a passo. O objetivo é servir como referência para quem vai implementar o lexer/tokenizer e o parser do shell.

## O que é parsing?

Parsing (análise sintática) é o processo de transformar uma sequência de caracteres (a linha que o usuário digitou) em uma estrutura de dados organizada (tokens e, em seguida, uma árvore ou lista de comandos) que o programa consegue entender e executar.

Etapas típicas:
- Lexing / Tokenização: dividir a string de entrada em tokens (palavras, operadores, redirecionadores, etc.).
- Parsing: interpretar a sequência de tokens e construir estruturas (por exemplo: comandos, argumentos, redirecionamentos, pipelines) de acordo com as regras da linguagem (neste caso, a sintaxe mínima do shell).
- Expansão: aplicar regras de expansão (variáveis `$VAR`, `$?`, globs se aplicável, e tratamento de quotes) antes ou durante o parsing, conforme o design.

## Para que serve o parsing neste projeto?

No `minishell` o parsing serve para:
- Transformar a entrada do usuário em uma representação manipulável (tokens e uma estrutura de comando).
- Detectar erros de sintaxe cedo (por exemplo `| |`, redirecionadores sem argumento, aspas não fechadas).
- Normalizar e aplicar regras de aspas/expansão para garantir que o executor receba as strings corretas.
- Separar responsabilidades: o executor recebe uma estrutura clara (comandos, args, redirecionamentos e conectores como pipes) e só cuida da execução, fork/exec/dup2.

Um parsing correto facilita o executor e reduz bugs ou comportamento inesperado.

## Requisitos funcionais do parser para minishell

- Reconhecer tokens:
  - Palavras (comando e argumentos)
  - Pipes: `|`
  - Redirecionadores: `<`, `>`, `>>`, `<<` (heredoc)
  - Operadores especiais se previstos (ainda que o enunciado restrinja o mínimo)
- Tratar aspas simples `'...'` (conteúdo literal, sem expansões)
- Tratar aspas duplas `"..."` (permitir expansão de `$` dentro)
- Fazer expansão de variáveis (`$VAR`, `$?`) nos locais adequados
- Garantir que o heredoc não grave linhas no histórico (`readline`)
- Gerar erros claros quando a sintaxe está incorreta (p. ex. `|` no começo/fim sem comando, redirecionador sem alvo)

## Contrato mínimo (entrada/saída do parser)

- Entrada: string (linha do prompt) e ambiente (variáveis para expansão).
- Saída: estrutura (por exemplo uma lista ligada de `command_t`) contendo:
  - `argv[]` (array de strings) — comando + argumentos após expansões e remoção de quotes
  - redirecionamentos: lista (tipo, filename, append flag)
  - ligação de pipes: ponteiro para próximo comando na pipeline
  - sucesso/erro de parsing (código e mensagem)

Estrutura mínima sugerida em C (exemplo):

```c
typedef enum e_redirect_type {
    R_IN,    // '<'
    R_OUT,   // '>'
    R_APPEND,// '>>'
    R_HEREDOC// '<<'
} t_redirect_type;

typedef struct s_redirect {
    t_redirect_type type;
    char *target; // filename or heredoc delimiter
    struct s_redirect *next;
} t_redirect;

typedef struct s_command {
    char **argv; // NULL-terminated
    t_redirect *redirects;
    struct s_command *next; // next command in pipeline
} t_command;
```

## Algoritmo recomendado (alto nível)

1. Lexer/tokenizer: transforme a linha em uma lista de tokens com tipos: WORD, PIPE, REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC, etc.
   - Implementar um state machine que reconhece:
     - palavras (respeitando escapes e aspas)
     - operadores `|`, `<`, `>`, `>>`, `<<`
   - Atenção às aspas: dentro de `'...'` tudo é literal; dentro de `"..."` o `$` ainda expande.

2. Parser: percorre os tokens e constrói as estruturas `t_command`:
   - Cria um novo `command` a cada token que representa um comando inicial (WORD quando não está no contexto de redirecionamento)
   - Para tokens de redirecionamento, consumir o próximo token (deve ser WORD) e criar um `t_redirect` ligado ao comando atual; gerar erro se faltar o alvo.
   - Para `PIPE`, finalizar o comando atual e iniciar um novo comando na lista ligada; gerar erro se tiver `PIPE` sem comando antes ou depois.

3. Expansões e remoção de quotes:
   - Ao gerar `argv` e `redirect->target`, aplicar expansão de variáveis e remover aspas.
   - Lembre-se: em `'single quotes'` não expandir; em `"double quotes"` expandir `$`.
   - `$?` deve ser substituído pelo último status (guardado num local apropriado).

4. Validação final:
   - Verifique consistência da lista (nenhum comando vazio), valide redirecionadores e retornos de erros sintáticos.

## Estratégias de implementação (passo a passo)

1. Escrever testes mínimos para o lexer:
   - `echo ola` -> tokens: WORD(echo), WORD(ola)
   - `ls | wc` -> WORD(ls), PIPE, WORD(wc)
   - `cat < file` -> WORD(cat), REDIR_IN, WORD(file)
   - `echo "a b"` -> WORD(echo), WORD(a b)

2. Implementar o lexer (arquivo `src/lexer.c`):
   - Função pública: `t_list *lexer_tokenize(const char *line);` ou `t_token *tokenize(const char *line);`
   - Representar tokens com tipo + string

3. Escrever testes para parser simples:
   - Transforme a sequência de tokens em `t_command` e verifique os campos: `argv`, `redirects`, `next`

4. Implementar parser em `src/parser.c`:
   - Função pública: `t_command *parse_tokens(t_token *tokens, int *errcode);`
   - Criar e popular as estruturas de comando

5. Implementar expansão/quote handling:
   - Funções utilitárias para:
     - remover aspas externas
     - expandir variáveis (consultando `char **envp` ou `environ`)
     - tratar `$?`

6. Integração com executor:
   - O executor deve aceitar `t_command *` e executar a pipeline correspondente, tratando redirecionamentos.

## Exemplo prático: tokenização de uma linha

Entrada:  echo "Olá $USER" > out.txt | wc -c

Tokens esperados (tipo : texto):
- WORD : "echo"
- WORD : "Olá $USER"   (a expansão pode ser aplicada depois; token captura o conteúdo entre aspas)
- REDIR_OUT : ">"
- WORD : "out.txt"
- PIPE : "|"
- WORD : "wc"
- WORD : "-c"

Após parsing e expansão (supondo USER=dev):
- Primeiro comando: argv = ["echo", "Olá dev", NULL], redirect out -> "out.txt"
- Segundo comando: argv = ["wc", "-c", NULL]

## Erros comuns e como detectá-los

- Aspas não fechadas: lexer detecta EOF sem fechar aspas -> erro de sintaxe
- Redirecionador sem alvo: depois de `<` ou `>` não vir um WORD -> erro
- Pipe no começo/fim: `| ls` ou `ls |` -> erro
- Uso incorreto de `<<` (heredoc) sem delimitador -> erro

## Testes recomendados

- Unit tests para o lexer com várias combinações de aspas/escapes
- Unit tests para o parser verificando estruturas resultantes
- Testes de integração simples: montar o `t_command`, chamar executor mock que só imprime estrutura e verificar saída esperada

## Observações sobre design e desempenho

- Prefira implementação simples e correta a otimizações prematuras.
- Use listas ligadas e `malloc`/`free` consistentemente; documente quem é responsável por liberar cada estrutura.
- Valide cada alocação e libere em caso de erro para evitar leaks.

## Exemplo de funções/arquivos sugeridos

- `src/lexer.c` / `include/lexer.h` — tokenizer e tipos de token
- `src/parser.c` / `include/parser.h` — constrói `t_command`
- `src/expander.c` / `include/expander.h` — funções para `$` e `?` e remoção de quotes
- `src/executor.c` / `include/executor.h` — executa `t_command` (fork/execve/dup2/pipe)
- `tests/lexer_tests.sh` e `tests/parser_tests.sh` — scripts de teste simples

## Checklist curto para implementação

- [ ] Implementar tokenizer (lexer)
- [ ] Implementar parser (construir `t_command`)
- [ ] Implementar expander (variáveis + $? + remover aspas)
- [ ] Testes unitários para lexer e parser
- [ ] Integrar com executor básico

---

Se quiser, eu posso:
- Gerar os stubs de arquivos (`src/lexer.c`, `src/parser.c`, `include/lexer.h`, `include/parser.h`) com as assinaturas das funções e estruturas sugeridas;
- Gerar testes iniciais em `tests/`;
- Implementar o lexer básico (estado inicial) e rodar testes localmente.

Diga qual das opções prefere que eu faça a seguir.
