#include "adventofcode.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PUZZLE_NAME "Day 11: Chronal Charge"

struct Answer {
  unsigned x;
  unsigned y;
  unsigned sq;
};

static inline int power_level(const unsigned x, const unsigned y,
                              const unsigned serial_number) {
  int rack_id = (int)x + 10;
  int power_level = rack_id * (int) y;
  power_level += serial_number;
  power_level *= rack_id;

  if (power_level > 100) {
    return ((power_level / 100) % 10) - 5;
  }

  return -5;
}

struct Answer solve_pt1(const unsigned serial_number) {
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

struct Answer solve_pt2(const unsigned serial_number) {
  int max = INT_MIN;
  struct Answer a;

  for (unsigned sq = 3; sq <= 16; sq++) {
    for (unsigned y = 0; y < 300 - sq; y++) {
      for (unsigned x = 0; x < 300 - sq; x++) {
        int pl = 0;
        for (unsigned yo = 0; yo < sq; yo++) {
          for (unsigned xo = 0; xo < sq; xo++) {
            pl += power_level(x + xo + 1, y + yo + 1, serial_number);
          }
        }

        if (pl > max) {
          max = pl;
          a.x = x;
          a.y = y;
          a.sq = sq;
        }
      }
    }
  }

  a.x++;
  a.y++;
  return a;
}

int main(void) {
  clock_t t = clock_time();

  char line[1024];
  fgets(line, 1024, stdin);

  unsigned serial_number = (unsigned) atoi(line);
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
