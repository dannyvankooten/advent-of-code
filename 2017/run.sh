#!/bin/bash

make clean
CXXFLAGS="-O2 -flto -march=native -mtune=native" make all

TIME_LINES=""
for DAY in {01..25}; do

    if [[ -f ./$DAY ]]; then
        OUT=$(cat $DAY.txt | ./$DAY)
        TIME_LINES+=$(echo -e "$OUT" | grep "Time: ")
        TIME_LINES+="\n"
        echo -e "$OUT\n"
    fi
done

# Print sum of runtimes
echo -e "$TIME_LINES" | awk 'BEGIN {sum=0.0} { gsub(/(Time: | μs)/,"", $2); sum += $2; } END { printf "Total time: %.2f μs\n", sum }'
