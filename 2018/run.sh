#!/bin/bash
TIME="0.0"
CC=${CC:=gcc}
echo "Using $($CC --version)"

for d in */; do 
    cd "$d"
    "$CC" -std=c11 -Wall -Werror -Ofast -march=native main.c -lcrypto 
    OUT=$(./a.out) 
    TIME_DAY=$(awk 'NR%4==0 { gsub(/ms$/,"", $2); print $2; }' <<< $OUT)
    TIME=$(echo "$TIME + $TIME_DAY" | bc)
    echo -e "$OUT\n"
    cd ..
done

printf "Total time: %.2fms\n", $TIME 

