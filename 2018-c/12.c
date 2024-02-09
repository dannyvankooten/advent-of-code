#include "adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 12: Subterranean Sustainability"
#define PADDING 200 // should be larger than # of cycles
#define INITIAL_PLANTS 99
#define WIDTH (INITIAL_PLANTS + PADDING * 2)

struct Pattern {
  char in[5];
  char out;
};

static unsigned long count_plants(const char plants[static WIDTH], const unsigned long offset) {
  unsigned long count = 0;

  // for part 2 I noticed the grid would stabilise
  // and then shifts right by 100 every 100 iterations
  // so I used that property to compute the answer...
  // not sure how it holds for other puzzle inputs though
  for (unsigned i = 0; i < WIDTH; i++) {
    if (plants[i] == '#') {
      count += (unsigned long)((long)i - (long)PADDING) + offset;
    }
  }

  return count;
}

static void growth_cycles(char plants[static WIDTH], const struct Pattern patterns[],
                   const unsigned npatterns, const unsigned ncycles) {
  char next[WIDTH];
  for (unsigned g = 0; g < ncycles; g++) {
    memset(next, '.', WIDTH * sizeof(char));

    // go over every plant
    for (unsigned p = 2; p < WIDTH - 2; p++) {
      for (unsigned r = 0; r < npatterns; r++) {
        if (memcmp(patterns[r].in, &plants[p - 2], 5) == 0) {
          next[p] = patterns[r].out;
          break;
        }
      }
    }

    memcpy(plants, next, WIDTH * sizeof(char));
  }
}

int main(void) {
  clock_t t = clock_time();

  char line[1024];
  fgets(line, 1024, stdin);

  // 20 generations
  // so 32 padding should be enough
  char plants[WIDTH];
  memset(plants, '.', WIDTH * sizeof(char));
  memcpy(&plants[PADDING], &line[15], INITIAL_PLANTS * sizeof(char));

  // skip 2nd line
  fgets(line, 1024, stdin);

  unsigned npatterns = 0;
  struct Pattern patterns[64];
  while (fgets(line, 1024, stdin) && npatterns < 64) {
    // we only need to know about the patterns that result into a plant
    if (line[9] != '#') {
      continue;
    }

    memcpy(patterns[npatterns].in, line, 5 * sizeof(char));
    patterns[npatterns].out = line[9];
    npatterns++;
  }

  unsigned long pt1 = 0;
  unsigned long pt2 = 0;
  growth_cycles(plants, patterns, npatterns, 20);
  pt1 = count_plants(plants, 0);
  growth_cycles(plants, patterns, npatterns, 180);
  pt2 = count_plants(plants, 49999999800ul);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %ld\n", pt1);
  printf("Part 2: %ld\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
