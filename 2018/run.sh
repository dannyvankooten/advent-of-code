#!/bin/bash

CC="${CC:=gcc}"
CFLAGS="$CFLAGS
-O2
-Wall
-Wextra
-Wpedantic
-std=c11
-Wformat=2
-Wconversion
-Wtrampolines
-Wimplicit-fallthrough
-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3
-D_GLIBCXX_ASSERTIONS
-fstrict-flex-arrays=3
-fstack-clash-protection -fstack-protector-strong
-Wl,-z,nodlopen -Wl,-z,noexecstack
-Wl,-z,relro -Wl,-z,now
-fPIE -pie"

$CC --version
ALLOUT=""
for d in */; do
    cd "$d"
    $CC $CFLAGS main.c -lcrypto
    OUT=$(./a.out)
    ALLOUT+="$OUT\n"
    echo -e "$OUT\n"
    cd ..
done

echo -e "$ALLOUT" | awk 'BEGIN {sum=0.0} NR%4==0 { gsub(/ms$/,"", $2); sum += $2; } END { printf "Total time: %.2fms\n", sum }'