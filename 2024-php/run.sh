#!/bin/bash

ALLOUT=""
for d in *.php; do
    OUT=$(php "$d")
    ALLOUT+="$OUT\n"
    echo -e "$OUT\n"
done

echo -e "$ALLOUT" | awk 'BEGIN {sum=0.0} NR%4==0 { gsub(/ms$/,"", $2); sum += $2; } END { printf "Total time: %.2fms\n", sum }'
