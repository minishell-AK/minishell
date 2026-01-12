#!/bin/bash
# heredoc edge cases runner for minishell
set -e
# test1: unquoted heredoc with dollar expansion literal
cat > /tmp/here_cmd1.in <<'SH'
cat <<EOF
no expansion $SHELL
EOF
exit
SH
./minishell < /tmp/here_cmd1.in > /tmp/here1.out 2>&1 || true
# test2: quoted delimiter (no expansion)
cat > /tmp/here_cmd2.in <<'SH'
cat <<'EOF'
$HOME
EOF
exit
SH
./minishell < /tmp/here_cmd2.in > /tmp/here2.out 2>&1 || true
# test3: command substitution-like content in heredoc
cat > /tmp/here_cmd3.in <<'SH'
cat <<EOF
$(echo X)
EOF
exit
SH
./minishell < /tmp/here_cmd3.in > /tmp/here3.out 2>&1 || true

echo "HEREDOC tests completed"
cat /tmp/here1.out /tmp/here2.out /tmp/here3.out
