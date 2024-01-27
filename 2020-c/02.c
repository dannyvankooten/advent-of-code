#include <assert.h>
#include <stdio.h>

#include "inputs/02.h"

int day2() {
  int count_valid = 0;
  const unsigned char *s = input;
  while (*s != '\0') {
    // read lower limit
    int lower_limit = 0;
    while (*s >= '0' && *s <= '9') {
      lower_limit = (lower_limit * 10) + (*s - '0');
      s++;
    }

    // '-'
    s++;

    // read higher limit
    int higher_limit = 0;
    while (*s >= '0' && *s <= '9') {
      higher_limit = (higher_limit * 10) + (*s - '0');
      s++;
    }

    // ' '
    s++;

    // read char requirement
    char c = *s++;

    // ": "
    s += 2;

    // read until "\n", check if c is between low and higher limit
    if ((s[lower_limit - 1] == c && s[higher_limit - 1] != c) ||
        (s[higher_limit - 1] == c && s[lower_limit - 1] != c)) {
      count_valid++;
    }

    while (*s != '\n' && *s != '\0') s++;
    if (*s == '\n') s++;
  }

  printf("%d\n", count_valid);
  assert(count_valid == 342);
  return 0;
}
