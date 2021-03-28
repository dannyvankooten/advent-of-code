#!/bin/bash
TOTAL=0.0
for dir in ./*/     
do
    dir=${dir%*/} 
    dir=${dir#"./"}     
    cd $dir
    cc *.c -Wall -Wextra -O2 -lm -march=native -o a.out
    start_time="$(date -u +%s.%N)"
    ./a.out > /dev/null
    end_time="$(date -u +%s.%N)"
    elapsed="$(bc <<<"$end_time-$start_time")"
    TOTAL="$(bc <<<"$TOTAL+$elapsed")"
    echo -e "Day $dir: \t$elapsed" "s"
    cd ..
done

echo -e "Total: \t\t$TOTAL" "s";
