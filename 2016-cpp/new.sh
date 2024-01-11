#!/usr/bin/env bash

if [[ "$AOCSESSION" == "" ]]; then
    echo "\$AOCSESSION environment variable not set."
    exit 1
fi;

DAY=$1
DAYNAME=$(printf "%02d\n" $DAY)
cp template.cpp $DAYNAME.cpp
wget --no-cookies --header="Cookie: session=$AOCSESSION" https://adventofcode.com/2016/day/$DAY/input -O $DAYNAME.txt
