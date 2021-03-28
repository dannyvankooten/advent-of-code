#!/bin/bash
TOTAL=0.0
for dir in ./*/     
do
    dir=${dir%*/}     
    cd $dir
    cc *.c -Wall -Wextra -O2 -lm -o a.out
    start_time="$(date -u +%s.%N)"
    ./a.out > /dev/null
    end_time="$(date -u +%s.%N)"
    elapsed="$(bc <<<"$end_time-$start_time")"
    TOTAL="$(bc <<<"$TOTAL+$elapsed")"
    echo "Day $dir: $elapsed" "s"
    cd ..
done

echo "Total runtime: $TOTAL" "s";
