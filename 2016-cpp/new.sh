#!/usr/bin/env bash

DAY=$1
DAYNAME=$(printf "%02d\n" $DAY)
cp template.cpp $DAYNAME.cpp
wget --no-cookies --header="Cookie: session=$AOCSESSION" https://adventofcode.com/2016/day/$DAY/input -O $DAYNAME.txt
