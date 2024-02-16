#!/usr/bin/env bash

set -e

YEAR=2019

if [[ "$AOCSESSION" == "" ]]; then
    echo "\$AOCSESSION environment variable not set."
    exit 1
fi;

DAY=$1
DAYNAME=$(printf "%02d\n" $DAY)
cp template.rs $DAYNAME.rs
wget --no-cookies --header="Cookie: session=$AOCSESSION" https://adventofcode.com/$YEAR/day/$DAY/input -O $DAYNAME.txt
