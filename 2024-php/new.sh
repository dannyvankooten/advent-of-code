#!/usr/bin/bash


if [[ "$AOCSESSION" == "" ]]; then
    echo "\$AOCSESSION environment variable not set."
    exit 1
fi;

DAY=$1
DAYNAME=$(printf "%02d\n" $DAY)

cp template.php "$DAYNAME.php"
sed -i "s/%%DAYNAME%%/$DAYNAME/" "$DAYNAME.php"
wget --no-cookies --header="Cookie: session=$AOCSESSION" https://adventofcode.com/2024/day/$DAY/input -O $DAYNAME.txt
