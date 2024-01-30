#include "adventofcode.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 11: Chronal Charge"

struct Answer {
  unsigned x;
  unsigned y;
  unsigned sq;
};

static inline int power_level(const unsigned x, const unsigned y,
                              const int serial_number) {
  int rack_id = (int)x + 10;
  int power_level = rack_id * (int)y;
  power_level += serial_number;
  power_level *= rack_id;

  if (power_level > 100) {
    return ((power_level / 100) % 10) - 5;
  }

  return -5;
}

struct Answer solve_pt1(const int serial_number) {
  int max = INT_MIN;
  unsigned xmax = 0;
  unsigned ymax = 0;
  for (unsigned y = 0; y < 300 - 3; y++) {
    for (unsigned x = 0; x < 300 - 3; x++) {
      int pl = 0;
      for (unsigned yo = 0; yo < 3; yo++) {
        for (unsigned xo = 0; xo < 3; xo++) {
          pl += power_level(x + xo + 1, y + yo + 1, serial_number);
        }
      }

      if (pl > max) {
        max = pl;
        xmax = x;
        ymax = y;
      }
    }
  }

  return (struct Answer){xmax + 1, ymax + 1, 3};
}

struct Answer solve_pt2(const int serial_number) {
  int max = INT_MIN;
  struct Answer a;

  int sum[301][301];
  memset(sum, 0, 301 * 301 * sizeof(**sum));

  // first pass: create a summed area table
  // see https://en.wikipedia.org/wiki/Summed-area_table
  for (unsigned y = 1; y <= 300; y++) {
    for (unsigned x = 1; x <= 300; x++) {
      int pl = power_level(x, y, serial_number);
      sum[y][x] = pl + sum[y - 1][x] + sum[y][x - 1] - sum[y - 1][x - 1];
    }
  }

  for (unsigned sq = 1; sq <= 300; sq++) {
    for (unsigned y = sq; y <= 300; y++) {
      for (unsigned x = sq; x <= 300; x++) {
        // to get sum of just this square
        // we can do the opposite as we did for calculating the summed area
        // which is: subtract adjacent corners, add opposite corner
        int total = sum[y][x] - sum[y - sq][x] - sum[y][x-sq] + sum[y-sq][x-sq];
        if (total > max) {
          max = total;
          a.x = x;
          a.y = y;
          a.sq = sq;
        }
      }
    }
  }

  a.x = a.x - a.sq + 1;
  a.y = a.y - a.sq + 1;

  return a;
}

int main(void) {
  clock_t t = clock_time();

  char line[1024];
  fgets(line, 1024, stdin);

  int serial_number = atoi(line);
  struct Answer pt1 = solve_pt1(serial_number);
  struct Answer pt2 = solve_pt2(serial_number);

  assert(power_level(3, 5, 8) == 4);
  assert(power_level(217, 196, 39) == 0);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d,%d\n", pt1.x, pt1.y);
  printf("Part 2: %d,%d,%d\n", pt2.x, pt2.y, pt2.sq);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
