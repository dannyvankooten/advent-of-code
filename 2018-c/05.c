#include "adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 5: Alchemical Reduction"

size_t react(char *input) {
  char *s = input + 1;
  size_t remaining_length = strlen(s);
  char o;
  while (*s != 0x0) {
    // determine opposite unit of same type
    if (*s >= 'a' && *s <= 'z') {
      o = *s - 32;
    } else {
      o = *s + 32;
    }

    if (*(s - 1) == o) {
      s--;
      remaining_length--;
      memmove(s, s + 2, remaining_length);
      *(s + remaining_length) = 0x0;
      continue;
    }

    s++;
    remaining_length--;
  }

  return strlen(input);
}

size_t pt2(char *input) {
  size_t shortest = 99999999;
  char input_copy[1024 * 64];

  for (char c = 'a'; c <= 'z'; c++) {
    strcpy(input_copy, input);
    char *s = input_copy;
    while (*s != 0x0) {
      if (*s == c || *s == c - 32) {
        size_t length = strlen(s) - 1;
        memmove(s, s + 1, length);
        *(s + length) = 0x0;
        continue;
      }
      s++;
    }

    size_t l = react(input_copy);
    if (l < shortest) {
      shortest = l;
    }
  }

  return shortest;
}

int main(void) {
  clock_t t = clock_time();
  char input[1024 * 64] = "";
  read_input_file(input, 1024 * 64, "05.txt");

  size_t a1 = react(input);
  size_t a2 = pt2(input);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %ld\n", a1);
  printf("Part 2: %ld\n", a2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
