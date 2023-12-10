#!/bin/bash

for d in */; do cd "$d" && cc -O2 -g main.c -lcrypto && valgrind --leak-check=full ./a.out && echo "" && cd ..; done
