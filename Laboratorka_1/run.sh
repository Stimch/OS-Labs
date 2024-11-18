#!/bin/bash
#laba_1

g++ parent.cpp -o parent
g++ child.cpp -o child

strace -o strace_output.txt -f ./parent