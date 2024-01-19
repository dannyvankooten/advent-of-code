#!/bin/bash

CC="${CC:=gcc}"
CFLAGS="$CFLAGS
-O2 -flto -march=native -mtune=native"

TIME_LINES=""
for d in */; do
    cd "$d"
    $CC $CFLAGS main.c -lcrypto
    OUT=$(./a.out)
    TIME_LINES+=$(echo -e "$OUT" | grep "Time: ")
    TIME_LINES+="\n"
    echo -e "$OUT\n"
    cd ..
done

# Print sum of runtimes
echo -e "$TIME_LINES" | awk 'BEGIN {sum=0.0} { gsub(/(Time: | μs)/,"", $2); sum += $2; } END { printf "Total time: %.3f ms\n", sum }'
