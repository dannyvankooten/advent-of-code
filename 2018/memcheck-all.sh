#!/bin/bash

for d in */; do 
    cd "$d"
    cc -O2 -Wall -Wextra -Wpedantic -std=c11 -g main.c -lcrypto
    echo "Running $d"
    valgrind --track-origins=yes -s --leak-check=full ./a.out 
    cd ..; 
done
