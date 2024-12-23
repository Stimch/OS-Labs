#!/bin/bash

# Компиляция управляющего узла
gcc -o controller controller.c

# Проверка, что компиляция прошла успешно
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Компиляция вычислительного узла
gcc -o compute_node compute_node.c
# Проверка, что компиляция прошла успешно
if [ $? -ne 0 ]; then
    echo "Compilation failed!"    
    exit 1
fi
# Запуск управляющего узла с помощью strace
strace -o strace-output.txt ./controller