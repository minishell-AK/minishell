#!/usr/bin/env python3
"""Run ./minishell under Valgrind in a PTY, send interactive commands including Ctrl-C, capture output."""
import os
import pty
import subprocess
import time
import select
import sys

LOG = "tests/logs/pty_valgrind.txt"
CMD = ["valgrind", "--leak-check=full", "--error-limit=no", "./minishell"]

def read_avail(master, timeout=0.1):
    rv = b""
    r, w, x = select.select([master], [], [], timeout)
    if r:
        try:
            rv = os.read(master, 4096)
        except OSError:
            rv = b""
    return rv

def main():
    master, slave = pty.openpty()
    proc = subprocess.Popen(CMD, stdin=slave, stdout=slave, stderr=slave, close_fds=True)
    os.close(slave)
    out = b""
    time.sleep(0.2)
    out += read_avail(master, 0.5)

    # Start a heredoc, send a line then Ctrl-C
    os.write(master, b'cat << oi\n')
    time.sleep(0.2)
    out += read_avail(master, 0.5)
    os.write(master, b'fjsdhf\n')
    time.sleep(0.2)
    out += read_avail(master, 0.5)
    os.write(master, b'\x03')  # Ctrl-C
    time.sleep(0.4)
    out += read_avail(master, 1.0)

    # export and echo
    os.write(master, b'export A=gjsfdhg\n')
    time.sleep(0.2)
    out += read_avail(master, 0.5)
    os.write(master, b'echo ${A}\n')
    time.sleep(0.2)
    out += read_avail(master, 0.5)

    # some commands
    os.write(master, b'fd\n')
    time.sleep(0.2)
    out += read_avail(master, 0.5)
    os.write(master, b'df\n')
    time.sleep(0.2)
    out += read_avail(master, 0.5)
    os.write(master, b'ls\n')
    time.sleep(0.2)
    out += read_avail(master, 0.5)

    # EOF to exit
    os.write(master, b'\x04')

    while proc.poll() is None:
        out += read_avail(master, 0.5)
        time.sleep(0.05)
    out += read_avail(master, 0.5)

    os.makedirs(os.path.dirname(LOG), exist_ok=True)
    with open(LOG, 'wb') as f:
        f.write(out)

    return proc.returncode

if __name__ == '__main__':
    sys.exit(main())
