#!/bin/bash
gcc parent.c -o parent
gcc child.c -o child
strace -o strace_output.txt -f ./parent
