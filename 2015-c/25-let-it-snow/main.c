#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void parse(const char *s, int *row, int *col) {
  s = skip_until_digit(s);
  s = parse_int(row, s);
  s = skip_until_digit(s);
  s = parse_int(col, s);
}

unsigned long value_at(int row, int col) {
  int r = 6;
  int c = 6;
  unsigned long v = 27995004;

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

int main(void) {
  clock_t start_t = clock_time();

  char input[1024];
  read_input_file(input, 1024, "input.txt");
  int row, col;
  parse(input, &row, &col);

  unsigned long a1 = value_at(row, col);
  printf("--- Day 25: Let It Snow ---\n");
  printf("Part 1: %ld\n", a1);
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}
