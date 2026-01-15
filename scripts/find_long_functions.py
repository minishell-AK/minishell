#!/usr/bin/env python3
import re
import os

def find_functions(path):
    results = []
    for root, dirs, files in os.walk(path):
        for f in files:
            if not f.endswith('.c'):
                continue
            fp = os.path.join(root, f)
            with open(fp, 'r', errors='ignore') as fh:
                lines = fh.readlines()
            i = 0
            n = len(lines)
            while i < n:
                line = lines[i]
                # skip comments and preprocessor
                if line.strip().startswith('//') or line.strip().startswith('#'):
                    i += 1
                    continue
                # look for a possible function signature ending with ')' possibly followed by space and '{' or next line '{'
                if ')' in line:
                    # join up to next 5 lines to match signatures that span lines
                    sig_block = ''.join(lines[i:i+6])
                    m = re.search(r"([a-zA-Z_][a-zA-Z0-9_\*\s]+)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\([^;]*\)\s*\{", sig_block)
                    if m:
                        func_start = i + sig_block[:m.start()].count('\n')
                        func_name = m.group(2)
                        # determine brace depth
                        depth = 0
                        j = func_start
                        started = False
                        while j < n:
                            l = lines[j]
                            for ch in l:
                                if ch == '{':
                                    depth += 1
                                    started = True
                                elif ch == '}':
                                    depth -= 1
                            j += 1
                            if started and depth == 0:
                                break
                        func_end = j
                        length = func_end - func_start
                        if length > 25:
                            rel = os.path.relpath(fp, os.getcwd())
                            results.append((rel, func_name, func_start+1, func_end, length))
                        i = func_end
                        continue
                i += 1
    return results

if __name__ == '__main__':
    paths = ['src', 'include']
    all_res = []
    for p in paths:
        if os.path.isdir(p):
            all_res.extend(find_functions(p))
    if not all_res:
        print('NO_LONG_FUNCTIONS')
    else:
        for r in all_res:
            print(f"{r[0]}:{r[1]}:start={r[2]}:end={r[3]}:lines={r[4]}")
