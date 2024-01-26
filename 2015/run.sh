#!/bin/bash

CC="${CC:=gcc}"
CFLAGS="$CFLAGS
-O2
-march=native
-mtune=native
-Wall
-Wextra
-Wpedantic
-std=c99
-Wformat=2
-Wconversion
-Wtrampolines
-Wimplicit-fallthrough
"

$CC --version
TIME_LINES=""
for d in */; do
    cd "$d"
    $CC $CFLAGS main.c -lcrypto
    OUT=$(./a.out)
    echo -e "$OUT\n"

    TIME_LINES+=$(echo -e "$OUT" | grep "Time: ")
    TIME_LINES+="\n"

    cd ..
done

echo -e "$TIME_LINES" | awk 'BEGIN {sum=0.0} { gsub(/(Time: | ms)/,"", $2); sum += $2; } END { printf "Total time: %.3f ms\n", sum }'
