#!/bin/bash

if [ "$#" -lt 1 ]; then
    echo "Provide a challenge number. Example: run 001"
    exit
fi

DIR=$1
shift 
cd "$DIR"
CFLAGS="$CFLAGS -DSTEP"

if [[ $* == *--debug* ]]; then
    cc main.c -Wall -O0 -g -lm 
    valgrind --leak-check=full -s ./a.out
    rm vgcore*
    rm a.out
elif [[ $* == *--cachegrind* ]]; then 
    cc main.c -Wall -O0 -g -lm 
    valgrind --tool=cachegrind -s ./a.out
    rm vgcore*
    rm a.out    
elif [[ $* == *--gdb* ]]; then 
    cc main.c -Wall -ggdb -O0 -lm -o a.out
    gdb a.out
    rm a.out
else 
    cc main.c -pedantic -Wall -Wextra -Ofast -lm 
    /usr/bin/time -f "\nWall time: %es\nMemory usage: %MKB" ./a.out
    rm a.out
fi;

cd ..