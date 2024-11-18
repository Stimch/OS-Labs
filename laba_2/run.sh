#!/bin/bash 
 
# Проверка наличия аргумента командной строки 
if [ "$#" -ne 1 ]; then 
    echo "Usage: $0 <max_threads>" 
    exit 1 
fi 
 
# Компиляция программы 
gcc -o max_triangle max_triangle.c -lpthread -lm 
 
# Проверка успешности компиляции 
if [ $? -ne 0 ]; then 
    echo "Compilation failed" 
    exit 1 
fi 
 
# Запуск программы с использованием strace 
strace -o strace_output.txt ./max_triangle $1 
 
# Проверка успешности выполнения программы 
if [ $? -ne 0 ]; then 
    echo "Program execution failed" 
    exit 1 
fi 
 
# Вывод количества потоков, используемых программой 
echo "Number of threads used: $1" 
 
echo "Program executed successfully. strace output is in strace_output.txt"
