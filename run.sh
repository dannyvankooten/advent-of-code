#!/bin/bash

if [ "$#" -lt 1 ]; then
    echo "Provide a challenge number. Example: run 001"
    exit
fi

cd $1

if [[ $* == *--debug* ]]; then
    cc main.c -g -Wall -std=c11 -lm 
    valgrind ./a.out
    rm vgcore*
    rm a.out
elif [[ $* == *--gdb* ]]; then 
    cc main.c -ggdb -O0 -Wall -std=c11 -lm -o a.out
    nemiver a.out
    rm a.out
else 
    cc main.c -O2 -Wall -Wpedantic -std=c11 -lm 
    /usr/bin/time -f "\nWall time: %es\nMemory usage: %MKB" ./a.out
    rm a.out
fi;

cd ..