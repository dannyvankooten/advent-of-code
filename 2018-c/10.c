#include "adventofcode.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 10: The Stars Align"

struct Point {
  int px, py;
  int vx, vy;
};

static void print(const struct Point points[static 512], const unsigned int npoints,
                  const int x1, const int y1, const int x2, const int y2) {
  unsigned w = (unsigned) (x2 - x1) + 1;
  unsigned h = (unsigned) (y2 - y1) + 1;

  assert(h <= 12);
  assert(w <= 64);
  char grid[12][64];
  memset(grid, ' ', 12 * 64 * sizeof(char));

  for (unsigned p = 0; p < npoints; p++) {
    unsigned y = (unsigned) (points[p].py - y1);
    unsigned x = (unsigned) (points[p].px - x1);
    assert(y < h && x < w);
    grid[y][x] = '#';
  }

  for (unsigned y = 0; y < h; y++) {
    for (unsigned x = 0; x < w; x++) {
      printf("%c", grid[y][x]);
    }
    printf("\n");
  }
}

int main(void) {
  clock_t t = clock_time();

  char line[1024];
  struct Point points[512];
  unsigned npoints = 0;
  while (fgets(line, 1024, stdin) && npoints < 512) {
    points[npoints].px = atoi(&line[10]);
    points[npoints].py = atoi(&line[18]);
    points[npoints].vx = atoi(&line[36]);
    points[npoints].vy = atoi(&line[39]);
    npoints++;
  }

  unsigned int pt2 = 1;
  int x1, y1, x2, y2;
  for (;; pt2++) {
    x1 = INT_MAX;
    y1 = INT_MAX;
    x2 = INT_MIN;
    y2 = INT_MIN;

    for (unsigned p = 0; p < npoints; p++) {
      points[p].px += points[p].vx;
      points[p].py += points[p].vy;

      x1 = points[p].px < x1 ? points[p].px : x1;
      y1 = points[p].py < y1 ? points[p].py : y1;
      x2 = points[p].px > x2 ? points[p].px : x2;
      y2 = points[p].py > y2 ? points[p].py : y2;
    }

    // detect whenever points are all close enough on their y-axes
    // generic enough for my puzzle input
    // alternatively, we could detect a trend reversal
    if ((y2 - y1) < 12) {
      break;
    }
  }

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: \n");
  print(points, npoints, x1, y1, x2, y2);
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
