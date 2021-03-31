#!/bin/bash

if [ "$#" -lt 1 ]; then
    echo "Provide a challenge number. Example: run 001"
    exit
fi

DIR=$1
shift 
cd "$DIR"
CC="${CC:=clang}" 
CFLAGS="$CFLAGS -DSTEP"

if [[ $* == *--debug* ]]; then
    cc *.c -Wextra -Wall -O0 -g -lm 
    valgrind --leak-check=full -s ./a.out
    rm vgcore*
    rm a.out
elif [[ $* == *--cachegrind* ]]; then 
    cc *.c -Wall -Wextra -g -lm 
    valgrind --tool=cachegrind -s ./a.out
    rm vgcore*
    rm a.out    
elif [[ $* == *--gdb* ]]; then 
    cc *.c -Wall -Wextra -ggdb -lm -o a.out
    gdb a.out
    rm a.out
elif [[ $* == *--profile ]]; then
    $CC *.c -Wall -Wextra -pg -lm -o a.out
    ./a.out
    gprof a.out gmon.out > prof_output
    cat prof_output
    rm a.out
else 
    $CC *.c -Wall -Wextra -Ofast -lm -march=native
    sudo nice -n -19 /usr/bin/time -f "\n$CC\nWall time: %es\nMemory usage: %MKB" ./a.out
    rm a.out
fi;

cd ..