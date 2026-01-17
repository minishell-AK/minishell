#!/usr/bin/env bash
# Minimal automated smoke tests for minishell (pipelines, heredocs, env, status)
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/minishell"
TMP_OUT="$ROOT_DIR/tmp_ms_out.txt"

cleanup_files() { rm -f "$TMP_OUT"; }
trap cleanup_files EXIT

afail() { echo "[FAIL] $1" >&2; exit 1; }

run_case() {
    local name="$1" input="$2" expected="$3"
    local out
    out="$(printf "%b" "$input" | script -q -c "$BIN" /dev/null)" || true
    for line in ${expected}; do
        if ! printf "%s" "$out" | grep -Fq "$line"; then
            printf "[FAIL] %s: missing '%s'\n" "$name" "$line" >&2
            printf "--- output ---\n%s\n------------\n" "$out" >&2
            exit 1
        fi
    done
    printf "[OK] %s\n" "$name"
}

# Cases
run_case "pipeline" \
    "echo PIPE | cat\nexit\n" \
    "PIPE"

run_case "pipeline-chain" \
    "echo A | tr A B | tr B C\nexit\n" \
    "C"

run_case "multi-heredoc" \
    "cat <<EOF\nhereline1\nEOF\ncat <<X\nhereline2\nX\nexit\n" \
    "hereline1 hereline2"

run_case "redir-out-in" \
    "rm -f $TMP_OUT\necho OUT > $TMP_OUT\ncat $TMP_OUT\nexit\n" \
    "OUT"

run_case "redir-append" \
    "rm -f $TMP_OUT\necho FIRST > $TMP_OUT\necho SECOND >> $TMP_OUT\ncat $TMP_OUT\nexit\n" \
    "FIRST SECOND"

run_case "env-and-status" \
    "export FOO=bar\necho \$FOO\nfalse\necho \$?\ntrue\necho \$?\nunset FOO\necho \$FOO\nexit\n" \
    "bar 1 0"

run_case "cmd-not-found" \
    "foobarbazcmd\necho \$?\nexit\n" \
    "127"

printf "All minishell smoke tests passed.\n"
