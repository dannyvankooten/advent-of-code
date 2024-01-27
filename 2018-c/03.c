#include "adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PUZZLE_NAME "Day 3: No Matter How You Slice It"

struct answer {
  int pt1;
  int pt2;
};

struct claim {
  int id;
  int offset_x;
  int offset_y;
  int width;
  int height;
};

int parse(struct claim *claims, const char *s) {
  int nclaims = 0;
  while (*s != 0x0) {
    struct claim *claim = &claims[nclaims++];
    s = skip("#", s);
    s = parse_int(&claim->id, s);
    s = skip(" @ ", s);
    s = parse_int(&claim->offset_x, s);
    s = skip(",", s);
    s = parse_int(&claim->offset_y, s);
    s = skip(": ", s);
    s = parse_int(&claim->width, s);
    s = skip("x", s);
    s = parse_int(&claim->height, s);
    if (*s == '\n')
      s++;
  }
  return nclaims;
}

struct answer solve(char *input) {
  struct claim *claims = malloc_or_die(2048 * sizeof(*claims));
  int nclaims = parse(claims, input);
  int *grid = malloc_or_die(1000 * 1000 * sizeof(int));
  memset(grid, 0, 1000 * 1000 * sizeof(int));
  struct answer answer = {0, 0};

  // 1st pass, mark all claims
  for (int n = 0; n < nclaims; n++) {
    struct claim *claim = &claims[n];
    for (int i = 0; i < claim->height; i++) {
      for (int j = 0; j < claim->width; j++) {
        grid[(claim->offset_y + i + 1) * 1000 + claim->offset_x + j + 1]++;
      }
    }
  }

  // for pt1, count all grid items with two or more claimed squares
  for (int y = 0; y < 1000; y++) {
    for (int x = 0; x < 1000; x++) {
      if (grid[y * 1000 + x] >= 2) {
        answer.pt1++;
      }
    }
  }

  // 2nd pass, identify claim with no overlaps
  for (int n = 0; n < nclaims; n++) {
    struct claim *claim = &claims[n];
    int overlaps = 0;
    for (int i = 0; i < claim->height && overlaps == 0; i++) {
      for (int j = 0; j < claim->width && overlaps == 0; j++) {
        int v =
            grid[(claim->offset_y + i + 1) * 1000 + claim->offset_x + j + 1];
        if (v >= 2) {
          overlaps = 1;
        }
      }
    }

    if (overlaps == 0) {
      answer.pt2 = claim->id;
    }
  }

  free(grid);
  free(claims);
  return answer;
}

int main(void) {
  clock_t t = clock_time();
  char input[1024 * 64];
  read_input_file(input, 1024 * 64, "03.txt");

  struct answer a = solve(input);
  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", a.pt1);
  printf("Part 2: %d\n", a.pt2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
