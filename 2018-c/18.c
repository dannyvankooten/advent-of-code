#include "adventofcode.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 18: Settlers of The North Pole"

static size_t parse(char *grid) {
  char *g = grid;
  char buf[1024];
  size_t len = 0;
  while (fgets(buf, 1024, stdin)) {
    len = strlen(buf) - 1;
    memcpy(g, buf, len);
    g += len;
  }
  return len;
}

struct Count {
  int trees;
  int lumberyards;
  int open;
};

static struct Count visit_adjacent(const char *grid, const size_t width,
                                   const unsigned int r1,
                                   const unsigned int c1) {
  struct Count cnt = {0, 0, 0};

  for (int rd = -1; rd <= 1; rd++) {
    for (int cd = -1; cd <= 1; cd++) {
      if (rd == 0 && cd == 0) {
        continue;
      }

      unsigned r2 = (unsigned)((int)r1 + rd);
      unsigned c2 = (unsigned)((int)c1 + cd);
      if (r2 >= width || c2 >= width) {
        continue;
      }

      switch (grid[r2 * width + c2]) {
      case '#':
        cnt.lumberyards++;
        break;
      case '|':
        cnt.trees++;
        break;
      case '.':
        cnt.open++;
        break;
      };
    }
  }

  return cnt;
}

static void step(const size_t width, char grid[width*width], char next[width*width]) {
  for (unsigned r = 0; r < width; r++) {
    for (unsigned c = 0; c < width; c++) {
      const struct Count cnt = visit_adjacent(grid, width, r, c);
      const size_t pos = r * width + c;
      switch (grid[pos]) {
      case '.':
        if (cnt.trees >= 3) {
          next[pos] = '|';
        } else {
          next[pos] = '.';
        }
        break;

      case '|':
        if (cnt.lumberyards >= 3) {
          next[pos] = '#';
        } else {
          next[pos] = '|';
        }
        break;

      case '#':
        if (cnt.trees >= 1 && cnt.lumberyards >= 1) {
          next[pos] = '#';
        } else {
          next[pos] = '.';
        }

        break;
      }
    }
  }

  // swap pointers
  // char *tmp = grid;
  memcpy(grid, next, width * width * sizeof(char));
  // grid = next;
  // next = tmp;
}

struct Cycle {
  /* index of first repetition */
  unsigned int mu;

  /* shortest cycle length */
  unsigned int lam;
};

/*
Floyd's tortoise and hare cycle-finding algorithm
See https://en.wikipedia.org/wiki/Cycle_detection
*/
static struct Cycle floyd(const size_t width, const char grid[width * width]) {
  const size_t n = width * width * sizeof(char);
  char *tortoise = malloc(n);
  char *hare = malloc(n);
  assert(tortoise != NULL);
  assert(hare != NULL);

  char *next = malloc(n);
  assert(next != NULL);

  /*
   Main phase of algorithm: finding a repetition x_i = x_2i.
   The hare moves twice as quickly as the tortoise and
   the distance between them increases by 1 at each step.
   Eventually they will both be inside the cycle and then,
   at some point, the distance between them will be
   divisible by the period λ.
  */
  memcpy(tortoise, grid, n);
  memcpy(hare, grid, n);
  step(width, tortoise, next);
  step(width, hare, next);
  step(width, hare, next);
  while (memcmp(tortoise, hare, n) != 0) {
    step(width, tortoise, next);
    step(width, hare, next);
    step(width, hare, next);
  }

  /*
  At this point the tortoise position, ν, which is also equal
  to the distance between hare and tortoise, is divisible by
  the period λ. So hare moving in cycle one step at a time,
  and tortoise (reset to x0) moving towards the cycle, will
  intersect at the beginning of the cycle. Because the
  distance between them is constant at 2ν, a multiple of λ,
  they will agree as soon as the tortoise reaches index μ.

  Find the position μ of first repetition.
  */
  unsigned mu = 0;
  memcpy(tortoise, grid, n);
  while (memcmp(tortoise, hare, n) != 0) {
    step(width, tortoise, next);
    step(width, hare, next);
    mu++;
  }

  /*
   Find the length of the shortest cycle starting from x_μ
   The hare moves one step at a time while tortoise is still.
   lam is incremented until λ is found.
  */
  unsigned lam = 1;
  step(width, hare, next);
  while (memcmp(tortoise, hare, n) != 0) {
    step(width, hare, next);
    lam++;
  }

  free(tortoise);
  free(hare);
  free(next);

  return (struct Cycle) { mu, lam};
}

static unsigned grid_value(const size_t width, const char grid[width*width]) {
  unsigned trees = 0;
  unsigned lumberyards = 0;
  for (unsigned r = 0; r < width; r++) {
    for (unsigned c = 0; c < width; c++) {
      const char tile = grid[r * width + c];
      switch (tile) {
      case '|': trees++; break;
      case '#': lumberyards++; break;
      }
    }
  }

  return trees * lumberyards;
}

static unsigned int nsteps(const size_t width, const char grid[width * width], const unsigned nsteps) {
  const size_t n = width * width * sizeof(char);
  char *cur = malloc( n);
  assert(cur != NULL);
  memcpy(cur, grid,  n);

  char *next = malloc( n);
  assert(next != NULL);

  for (unsigned int i = 0; i < nsteps; i++) {
    step(width, cur, next);
  }

  unsigned int value = grid_value(width, cur);
  free(cur);
  free(next);
  return value;
}

int main(void) {
  clock_t t = clock_time();

  size_t grid_size = 50 * 50;
  char *grid = malloc(grid_size * sizeof(char));
  assert(grid != NULL);
  const size_t width = parse(grid);

  const unsigned int pt1 = nsteps(width, grid, 10);

  /* Detect cycle for part 2 */
  const struct Cycle c = floyd(width, grid);
  const unsigned int pt2 = nsteps(width, grid, c.mu + ((1000000000u - c.mu) % c.lam));

  free(grid);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", pt1);
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
