#!/usr/bin/env bash

DAY=$1
DAYNAME=$(printf "%02d\n" $DAY)
cp template.cpp $DAYNAME.cpp
wget --no-cookies --header="Cookie: session=53616c7465645f5fc3aa4e4427dbe870e15d894a1e8b30106fceaeaef39f4fe067b823862fe479561ea5e48116259b841294f3ad2c43a796b2fb58bfb3739a32" https://adventofcode.com/2016/day/$DAY/input -O $DAYNAME.txt
