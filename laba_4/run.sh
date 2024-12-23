#!/bin/bash

# Компиляция динамических библиотек
gcc -shared -o libe.so -fPIC libe.c -lm
gcc -shared -o libe_series.so -fPIC libe_series.c -lm
gcc -shared -o libtranslation_binary.so -fPIC libtranslation_binary.c
gcc -shared -o libtranslation_ternary.so -fPIC libtranslation_ternary.c

# Компиляция тестовых программ
gcc -o program1 program1.c -ldl -lm
gcc -o program2 program2.c -ldl -lm

# Запуск программы No1 с strace
echo "Running program1 with strace..."
strace -o strace-output.txt ./program1 <<EOF
1 5
2 10
q
EOF

# Запуск программы No2 с strace
echo "Running program2 with strace..."
strace -o strace-output.txt ./program2 <<EOF
1 5
0
1 5
2 10
0
2 10
q
EOF

echo "Done. Check strace-output.txt for strace output."
