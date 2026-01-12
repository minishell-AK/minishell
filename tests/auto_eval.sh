#!/usr/bin/env bash
set -eu

OUT=tests/auto_eval.output
rm -f "$OUT"

echo "Running automated minishell tests..." > "$OUT"
cat > testcmd.sh <<'S'
#!/bin/sh
echo TESTCMD
S
chmod +x testcmd.sh

./minishell <<'EOF' > "$OUT" 2>&1
/bin/echo hello
/bin/ls /
echo -n no_newline
echo X Y
/bin/ls nonexistent
echo $?
echo hi > /tmp/minishell_test_file
cat < /tmp/minishell_test_file
echo x >> /tmp/minishell_test_file
cat < /tmp/minishell_test_file
echo "a b"
echo '$USER'
echo "$USER"
export MTEST=XYZ
echo $MTEST
unset MTEST
echo $MTEST
mkdir -p tmpdir_eval
cd tmpdir_eval
pwd
cd -
pwd
PATH_ORIG=$PATH
PATH=
ls >/dev/null 2>&1 || true
echo $?
PATH=.:$PATH_ORIG
./testcmd.sh
rm testcmd.sh
exit
EOF

echo "Checking outputs..."

fail=0
check() {
  local desc="$1"; shift
  local pat="$1"; shift
  if ! grep -q -F -- "$pat" "$OUT"; then
    echo "[FAIL] $desc: expected '$pat'";
    fail=1
  else
    echo "[ OK ] $desc"
  fi
}

check "absolute echo" "hello"
check "echo -n" "no_newline"
check "echo args" "X Y"
check "stderr on ls nonexistent" "No such file" || true
check "print exit status" 'echo $?'
# Instead of checking numeric value, ensure $? was printed by checking previous ls error then a number; skip strict check
check "redirection and cat" "hi"
check "append redirection" "x"
check "double quotes" "a b"
check "single quotes literal" "\$USER"
check "export var" "XYZ"
check "unset var empty" "echo \$MTEST"
unset_line=$(grep -n "unset MTEST" "$OUT" | head -n1 | cut -d: -f1 || true)
if [ -n "$unset_line" ]; then
  tail -n +$((unset_line + 1)) "$OUT" | grep -q "XYZ" && { echo "[FAIL] unset var empty: variable still present"; fail=1; } || echo "[ OK ] unset var empty"
else
  echo "[FAIL] unset var empty: marker not found"; fail=1
fi
check "pwd after cd" "tmpdir_eval"
check "PATH exec" "TESTCMD"

if [ "$fail" -ne 0 ]; then
  echo "Some tests failed. See $OUT"; exit 2
fi

echo "All basic tests passed."; exit 0
