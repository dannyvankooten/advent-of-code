#include "../adventofcode.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void parse(const char *s, int *row, int *col) {
  s = skip_until_digit(s);
  s = parse_int(row, s);
  s = skip_until_digit(s);
  s = parse_int(col, s);
}

uint64_t value_at(int row, int col) {
  int r = 6;
  int c = 6;
  uint64_t v = 27995004;

  for (; r < row || c < col;) {
    if (r == 1) {
      r = c + 1;
      c = 1;
    } else {
      r -= 1;
      c += 1;
    }

    v = (v * 252533) % 33554393;
  }
  return v;
}

int main() {
  clock_t start_t = clock_time();

  char input[1024];
  read_input_file(input, 1024, "input.txt");
  int row, col;
  parse(input, &row, &col);

  int a1 = value_at(row, col);
  int a2 = 0;
  printf("--- Day 23: Opening the Turing Lock ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}