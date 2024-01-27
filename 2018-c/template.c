#include "adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PUZZLE_NAME "Day 10: Knot Hash"

int main(void) {
  clock_t t = clock_time();
  char input[1024 * 64];
  read_input_file(input, 64 * 1024, "input.txt");

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", 0);
  printf("Part 2: %d\n", 0);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
