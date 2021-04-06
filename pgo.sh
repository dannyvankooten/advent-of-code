#/bin/bash

rm aoc2020
CFLAGS="-fprofile-generate -g" make release
./aoc2020 --bench &> /dev/null
rm aoc2020
CFLAGS=-fprofile-use make release