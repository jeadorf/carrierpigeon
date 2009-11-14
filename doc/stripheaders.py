#!/usr/bin/env python
#
# 
"""Extracts all function declarations from a header file and writes them
   back to a 'stripped' header file.
 
   Input: a list of filenames (not in the current directory!!)
   Output: the stripped header files (in the current directory)"""

import sys
import os
import re

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "-static":
        func = re.compile("static [\w ]+\([\w\*, ]*\); *$")
    else:
        func = re.compile("[\w ]+\([\w\*, ]*\); *$")
        
    for p in (p.rstrip() for p in sys.stdin):
        fin = open(p, "r")
        fout = open(os.path.basename(p), "w")
        fout.writelines(
            (s + os.linesep for s in 
            (m.group() for m in 
            (func.match(line) for line in fin) if m != None)))
        fin.close()
        fout.close()

