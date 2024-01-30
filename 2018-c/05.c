#include "adventofcode.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 5: Alchemical Reduction"

size_t react(char *input) {
  size_t total_length = strlen(input);
  size_t remaining_length = total_length - 1;
  unsigned int i = 1;
  int o;
  while (input[i] != 0x0) {
    o = islower(input[i]) ? toupper(input[i]) : tolower(input[i]);

    if (i > 0 && input[i-1] == o) {
      i--;
      memmove(&input[i], &input[i+2], remaining_length);
      remaining_length -= 1;
      total_length -= 2;
      continue;
    }

    i++;
    remaining_length--;
  }

  return total_length;
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
