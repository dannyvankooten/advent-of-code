#!/bin/bash

TIME="0.0"
for d in */; do 
    cd "$d"
    cc -Ofast -march=native main.c -lcrypto 
    OUT=$(./a.out) 
    TIME_DAY=$(awk 'NR%4==0 { gsub(/ms$/,"", $2); print $2; }' <<< $OUT)
    TIME=$(echo "$TIME + $TIME_DAY" | bc)
    echo -e "$OUT\n"
    cd ..
done

printf "Total time: %.2fms\n", $TIME 

