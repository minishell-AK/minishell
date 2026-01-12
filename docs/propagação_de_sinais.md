# Propagação de sinais (implementação)

Este documento descreve o que foi implementado no executor para que sinais (SIGINT, SIGQUIT) sejam repassados para pipelines de forma semelhante ao comportamento do bash, e onde o código relevante está localizado.

O que foi feito
- Cada processo filho criado para executar um comando em um pipeline agora chama `setpgid(0, 0)` logo no início do código do filho. Isso garante que cada filho crie/entre em um grupo de processos próprio.
- O pai organiza todos os filhos do pipeline para compartilharem o mesmo process group (pgid). Implementação: após cada `fork()` bem-sucedido o pai chama `setpgid(pid_filho, pgid_do_primeiro_filho)` para agrupar os processos do pipeline.
- Antes de aguardar os filhos, o shell coloca o grupo do pipeline em foreground com `tcsetpgrp(STDIN_FILENO, pgid)` (somente quando `stdin` é um tty), de modo que sinais de terminal (Ctrl-C, Ctrl-\) sejam entregues ao grupo do pipeline.
- Cada filho restaura os handlers de sinal para o comportamento padrão (`SIG_DFL`) para que receba sinais normalmente enquanto estiver em foreground.
- Depois que o pipeline termina, o shell restaura o seu próprio process group ao foreground com `tcsetpgrp(STDIN_FILENO, getpid())` (novamente apenas se `stdin` for tty).

Onde está o código
- Arquivo principal modificado: `src/exec/exe_cmd.c` — este arquivo agora:
	- chama `setpgid(0, 0)` e `signal(SIGINT, SIG_DFL)` / `signal(SIGQUIT, SIG_DFL)` nos filhos;
	- no pai usa `setpgid()` para agrupar filhos e `tcsetpgrp()` para colocar o pipeline em foreground antes de `waitpid()`;
	- restaura o foreground do terminal ao PID do shell depois que os filhos terminam.

Detalhes de robustez
- As chamadas a `tcsetpgrp()` são condicionais a `isatty(STDIN_FILENO)` para evitar erros quando o shell não estiver rodando em um terminal interativo (por exemplo, em execuções não interativas ou em testes automatizados).
- Erros simples de `setpgid()` não interrompem o fluxo; em ambientes com restrições de permissões ou quando o kernel retorna EACCES/EPERM, o comportamento cai para o modo sem alteração de pgid (o shell ainda espera os filhos normalmente). Se desejar, posso adicionar tratamento explícito de erros e retries.

Testes rápidos realizados
- Recompilei o projeto e rodei um pipeline simples interativo: `echo hi | cat` — o pipeline produziu saída correta e o prompt foi restaurado.
- Também testei execução em modo não interativo (script heredado) para garantir que não há chamadas indevidas a `tcsetpgrp()` quando `stdin` não é tty.

Próximos passos sugeridos
- Adicionar tratamento de erros mais detalhado em `setpgid()`/`tcsetpgrp()` e logs opcionais.
- Criar testes automatizados que verifiquem comportamento de sinais (uso de ptys ou scripts que enviam sinais aos pids do pipeline).

Se quiser, eu acrescento exemplos de testes automatizados e atualizo `docs/COMO_COMECAR_MINISHELL.md` para referenciar esta página.

