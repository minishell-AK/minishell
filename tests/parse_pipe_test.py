#!/usr/bin/env python3
import os, pty, time, select
MASTER_BUF=1024
master,slave=pty.openpty()
pid=os.fork()
if pid==0:
    os.setsid()
    os.dup2(slave,0);os.dup2(slave,1);os.dup2(slave,2)
    if slave>2: os.close(slave)
    os.execv('./minishell',['./minishell'])
os.close(slave)

def read_all(timeout=0.5):
    out=b''
    end=time.time()+timeout
    while time.time()<end:
        r,_,_=select.select([master],[],[],0.05)
        if master in r:
            try:
                d=os.read(master,MASTER_BUF)
            except OSError:
                break
            if not d: break
            out+=d
    return out

# wait prompt
time.sleep(0.2)
print(read_all().decode(errors='ignore'))
# send bad pipeline
os.write(master,b"ls | | ls\n")
time.sleep(0.2)
print(read_all().decode(errors='ignore'))
# exit shell
os.write(master,b"exit\n")
time.sleep(0.1)
print(read_all().decode(errors='ignore'))
