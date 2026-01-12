#!/usr/bin/env python3
import os, pty, time, signal, sys

def main():
    pid, fd = pty.fork()
    if pid == 0:
        os.execvp("./minishell", ["./minishell"])
    else:
        time.sleep(0.2)
        os.write(fd, b"cat <<EOF\n")
        time.sleep(0.2)
        # send SIGINT to the child's process group
        try:
            os.killpg(pid, signal.SIGINT)
        except Exception as e:
            print('killpg error', e, file=sys.stderr)
        time.sleep(0.2)
        os.write(fd, b"echo after_abort\n")
        os.write(fd, b"exit\n")
        out = b""
        try:
            while True:
                data = os.read(fd, 1024)
                if not data:
                    break
                out += data
        except OSError:
            pass
        os.waitpid(pid, 0)
        sys.stdout.write(out.decode(errors='ignore'))

if __name__ == '__main__':
    main()
