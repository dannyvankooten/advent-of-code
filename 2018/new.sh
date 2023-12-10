#!/bin/bash

set -e

mkdir $1
touch $1/main.c
touch $1/input.txt
touch $1/input_test.txt

cat template.c > $1/main.c
touch $1/README.txt

cd "$1"
