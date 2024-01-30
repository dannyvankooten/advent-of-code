#!/bin/bash
set -e

RELEASE=1 make all

TIME_LINES=""
for PROGRAM in build/*; do
    DAY=$(basename $PROGRAM)
    OUT=$(./$PROGRAM < $DAY.txt)
    TIME_LINES+=$(echo -e "$OUT" | grep "Time: ")
    TIME_LINES+="\n"
    echo -e "$OUT\n"
done

# Print sum of runtimes
echo -e "$TIME_LINES" | awk 'BEGIN {sum=0.0} { gsub(/(Time: | μs)/,"", $2); sum += $2; } END { printf "Total time: %.3f ms\n", sum }'
