#!/bin/bash

for d in */; do cd $d && go run main.go && echo "" && cd ..; done
(for d in */; do cd $d && go run main.go && cd ..; done) | awk 'BEGIN {sum=0.0} NR%4==0 { gsub(/ms$/,"", $2); sum += $2; } END { printf "Total time: %.2fms\n", sum }'
