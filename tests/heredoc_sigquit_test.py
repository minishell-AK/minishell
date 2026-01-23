#!/usr/bin/env python3
import os
import pty
import time
import select
import signal
import sys

MASTER_BUF = 1024

master, slave = pty.openpty()
# fork and exec minishell
pid = os.fork()
if pid == 0:
    os.setsid()
    os.dup2(slave, 0)
    os.dup2(slave, 1)
    os.dup2(slave, 2)
    if slave > 2:
        os.close(slave)
    os.execv('./minishell', ['./minishell'])
# parent
os.close(slave)

def read_until(timeout=1.0):
    out = b''
    end = time.time() + timeout
    while time.time() < end:
        r, _, _ = select.select([master], [], [], 0.1)
        if master in r:
            try:
                data = os.read(master, MASTER_BUF)
            except OSError:
                break
            if not data:
                break
            out += data
    return out

# wait for prompt
time.sleep(0.2)
print(read_until(0.5).decode(errors='ignore'))
# send heredoc start
os.write(master, b"cat <<EOF\n")
time.sleep(0.1)
print(read_until(0.5).decode(errors='ignore'))
# Now send SIGQUIT to foreground pgrp of pty
try:
    pgrp = os.tcgetpgrp(master)
    print('pty pgrp:', pgrp)
    os.killpg(pgrp, signal.SIGQUIT)
except Exception as e:
    print('failed to send SIGQUIT:', e)

# allow time and read
time.sleep(0.2)
print(read_until(0.5).decode(errors='ignore'))
# now send the delimiter and exit
os.write(master, b"EOF\n")
time.sleep(0.2)
print(read_until(1.0).decode(errors='ignore'))

# cleanup
try:
    os.kill(pid, signal.SIGTERM)
except Exception:
    pass

print('done')
