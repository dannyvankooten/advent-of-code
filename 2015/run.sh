#!/bin/bash

CC="${CC:=gcc}"
CC="/home/danny/gcc10/bin/gcc"
CFLAGS="$CFLAGS -Wall -Wextra -Wpedantic -std=c11 -Ofast -march=native    -Wimplicit-function-declaration
                                                                          -Wimplicit-int
                                                                          -Wint-conversion
                                                                          -Wreturn-type
                                                                          --warn-missing-parameter-type
                                                                          -Wincompatible-pointer-types
                                                                          -fanalyzer"
$CC --version
TIME="0.0"
for d in */; do
    cd "$d"
    $CC $CFLAGS main.c -lcrypto
    OUT=$(./a.out)
    TIME_DAY=$(awk 'NR%4==0 { gsub(/ms$/,"", $2); print $2; }' <<< $OUT)
    TIME=$(echo "$TIME + $TIME_DAY" | bc)
    echo -e "$OUT\n"
    cd ..
done

printf "Total time: %.2fms\n", $TIME

