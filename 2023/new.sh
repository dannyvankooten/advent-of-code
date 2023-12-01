#!/usr/bin/bash

mkdir $1
touch $1/main.go
touch $1/input.txt
touch $1/input_test.txt

cat template_main.go > $1/main.go
cat template_main_test.go > $1/main_test.go
touch $1/README.txt

cd "$1"
