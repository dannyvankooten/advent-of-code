#!/bin/bash

CC="${CC:=gcc}"
CFLAGS="$CFLAGS -Wall -Wextra -Wpedantic -std=c11 -Ofast -march=native -fanalyzer -Wlarger-than-524288 -Wundef -Winline "
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

