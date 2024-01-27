#!/bin/bash
set -e

make clean
RELEASE=1 make all

TIME_LINES=""
for DAY in build/*; do
    OUT=$(./$DAY)
    TIME_LINES+=$(echo -e "$OUT" | grep "Time: ")
    TIME_LINES+="\n"
    echo -e "$OUT\n"
done

# Print sum of runtimes
echo -e "$TIME_LINES" | awk 'BEGIN {sum=0.0} { gsub(/(Time: | μs)/,"", $2); sum += $2; } END { printf "Total time: %.3f ms\n", sum }'
