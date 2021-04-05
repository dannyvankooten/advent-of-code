#/bin/bash

make clean
CFLAGS="-fprofile-generate -g" make release
./aoc2020 --bench &> /dev/null
make clean
CFLAGS=-fprofile-use make release