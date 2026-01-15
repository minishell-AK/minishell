#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
LOGDIR="$ROOT/tests/logs"
mkdir -p "$LOGDIR"

echo "[BATTERY] Build"
cd "$ROOT"
make -j2 >/dev/null

echo "[BATTERY] Non-interactive tests"
# simple non-interactive session: list, env, export, unset
printf "ls\necho hello\nexport B=xyz\necho \${B}\nunset B\nexit\n" | ./minishell > "$LOGDIR/non_interactive.txt" 2>&1 || true

echo "[BATTERY] PTY interactive Valgrind test"
python3 "$ROOT/scripts/pty_valgrind.py" || true

echo "[BATTERY] Logs saved to $LOGDIR"
ls -la "$LOGDIR"
