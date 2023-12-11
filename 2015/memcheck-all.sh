#!/bin/bash

set -e

for d in */; do
  cd "$d"
  cc -O2 -Wall -Werror -Wpedantic -g main.c -lcrypto
  valgrind -s --track-origins=yes --leak-check=full ./a.out
  echo ""
  read -n 1 -p "Next? [Any key to proceed]"
  cd ..
done
