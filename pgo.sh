#/bin/bash

make clean
CFLAGS=-fprofile-generate make release
./aoc2020 --bench &> /dev/null
make clean
CFLAGS=-fprofile-use make release