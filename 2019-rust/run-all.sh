#!/bin/bash
set -e

TIME_LINES=""
for DIR in ./day-*; do
    cd $DIR
    OUT=$(cargo run --release --quiet < input.txt &2> /dev/null)
    TIME_LINES+=$(echo -e "$OUT" | grep "Time: ")
    TIME_LINES+="\n"

    echo -e "$DIR"
    echo -e "$OUT\n"
    cd ..
done

# Print sum of runtimes
echo -e "$TIME_LINES" | awk 'BEGIN {sum=0.0} { gsub(/(Time: | μs)/,"", $2); sum += $2; } END { printf "Total time: %.3f ms\n", sum }'
