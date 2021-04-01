#!/bin/bash
TOTAL=0.0
for DIR in ./*/     
do
    DIR=${DIR%*/} 
    DIR=${DIR#"./"}     
    cd $DIR
    BEST_ELAPSED=1000
    for CC in gcc clang; do
        for OPTLEVEL in O2 O3 Ofast; do
            $CC *.c -$OPTLEVEL -lm -march=native -o $DIR.$CC 
            for i in {1..10}; do
                START_TIME="$(date -u +%s.%N)"
                ./$DIR.$CC > /dev/null
                END_TIME="$(date -u +%s.%N)"
                ELAPSED="$(bc <<<"$END_TIME-$START_TIME")"
                if [[ $(bc <<< "$ELAPSED<$BEST_ELAPSED") ]]; then 
                    BEST_ELAPSED=$ELAPSED
                fi
            done
        done
    done
    
    TOTAL="$(bc <<<"$TOTAL+$BEST_ELAPSED")"
    echo -e "Day $DIR: \t$BEST_ELAPSED" "s"
    cd ..
done

echo -e "Total: \t\t$TOTAL" "s";
