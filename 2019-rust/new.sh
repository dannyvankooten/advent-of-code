#!/usr/bin/env bash

set -e

YEAR=2019

if [[ "$AOCSESSION" == "" ]]; then
    echo "\$AOCSESSION environment variable not set."
    exit 1
fi;

DAY=$1
DAYNAME=$(printf "%02d\n" $DAY)
DIR="day-$DAYNAME"
cargo new "$DIR"
cp template.rs "$DIR/src/main.rs"
wget --no-cookies --header="Cookie: session=$AOCSESSION" https://adventofcode.com/$YEAR/day/$DAY/input -O "$DIR/input.txt"
echo "input.txt" >> "$DIR/.gitignore"
echo "target/" >> "$DIR/.gitignore"
