#!/bin/bash

for d in */; do 
    cd "$d" 
    cc -Wall -Wextra -Wpedantic -std=c11 -g main.c -lcrypto
    valgrind -s --track-origins=yes --leak-check=full ./a.out 
    echo "" 
    cd ..
done
