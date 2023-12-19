#include "../adventofcode.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PUZZLE_NAME "Day 1: Chronal Calibration"

int pt1(const char *s) {
  int freq = 0;
  while (*s != '\0') {
    int n;
    s = parse_int(&n, s);
    freq += n;

    if (*s == '\n')
      s++;
  }
  return freq;
}

int pt2(char *input) {
  int freq = 0;
  uint8_t seen[256 * 1024] = {0};

  for (;;) {
    const char *s = input;
    int hash;
    int n;

    while (*s != '\0') {
      s = parse_int(&n, s);
      freq += n;
      hash = freq + 128 * 1024;
      if (seen[hash] == 1) {
        return freq;
      }
      seen[hash] = 1;

      if (*s == '\n')
        s++;
    }
  }
}

int main() {
  clock_t t = clock_time();
  char input[1024 * 64];
  read_input_file(input, 1024 * 64, "input.txt");

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", pt1(input));
  printf("Part 2: %d\n", pt2(input));
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}