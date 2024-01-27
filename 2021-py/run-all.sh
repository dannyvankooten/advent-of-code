#!/bin/bash 

set -e 

TIME="0.0"
for d in */; do
    cd "$d"
    python3 main.py
    cd ..
done