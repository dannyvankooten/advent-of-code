#!/bin/bash

ALLOUT=""
for d in */; do
    cd "$d"
    OUT=$(go run main.go)
    ALLOUT+="$OUT\n"
    echo -e "$OUT\n"
    cd ..
done

echo -e "$ALLOUT" | awk 'BEGIN {sum=0.0} NR%4==0 { gsub(/ms$/,"", $2); sum += $2; } END { printf "Total time: %.2fms\n", sum }'