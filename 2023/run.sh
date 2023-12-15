#!/bin/bash

set -e

TIME="0.0"
for d in */; do
    cd "$d"
    OUT=$(go run main.go)
    TIME_DAY=$(awk 'NR%4==0 { gsub(/ms$/,"", $2); print $2; }' <<< $OUT)
    TIME=$(echo "$TIME + $TIME_DAY" | bc)
    echo -e "$OUT\n"
    cd ..
done

printf "Total time: %.2fms\n", $TIME