#include "../adventofcode.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static unsigned int parse(const char *s, int weights[]) {
  unsigned int n = 0;
  while (*s != 0x0) {
    s = parse_int(&weights[n++], s);
    s = skip_optional('\n', s);
  }
  return n;
}

// qsort callback
static int cmp_ints(const void *p1, const void *p2) {
  const int a = *(int *)p1;
  const int b = *(int *)p2;
  return (a > b) + (a < b);
}

// this is sloppy, but it was a property of my input so why not?
static unsigned long solve_pt1(const int weights[], const unsigned int nweights,
                               const int group_weight) {
  unsigned long qe = 1;

  int w = 0;
  for (unsigned int i = 0; i < nweights && w < group_weight; i++) {
    if (w + weights[i] <= group_weight) {
      w += weights[i];
      qe *= (unsigned long)weights[i];
    }
  }

  assert(w == group_weight);

  return qe;
}

static unsigned long sumto(const int weights[], const unsigned int nweights,
                           unsigned int i, int sum, unsigned long qe,
                           const int want) {
  if (sum >= want) {
    return sum == want ? qe : 1ul << 63;
  }

  unsigned long best = 1ul << 63;
  unsigned long r;
  for (; i < nweights; i++) {
    r = sumto(weights, nweights, i + 1, sum + weights[i],
              qe * (unsigned long)weights[i], want);
    if (r < best) {
      best = r;
    }
  }
  return best;
}

int main(void) {
  clock_t start_t = clock_time();

  char input[1024];
  read_input_file(input, 1024, "input.txt");
  int weights[32];
  unsigned int nweights = parse(input, weights);
  qsort(weights, nweights, sizeof(int), cmp_ints);
  int weights_sum = 0;
  for (unsigned int i = 0; i < nweights; i++) {
    weights_sum += weights[i];
  }

  unsigned long pt1 = solve_pt1(weights, nweights, weights_sum / 3);
  unsigned long pt2 = sumto(weights, nweights, 0, 0, 1, weights_sum / 4);

  printf("--- Day 24: It Hangs in the Balance ---\n");
  printf("Part 1: %ld\n", pt1);
  printf("Part 2: %ld\n", pt2);
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}
