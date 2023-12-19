#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int parse(const char *s, int weights[]) {
  int n = 0;
  while (*s != 0x0) {
    s = parse_int(&weights[n++], s);
    s = skip_optional('\n', s);
  }
  return n;
}

struct config {
  int qe;
  int weights[3];
};

struct group {
  int weights[8];
  int nweights;
};

void solve(int weights[], struct group groups[3], int nweights) {
  for (int i = 0; i < nweights; i++) {
    for (int j = 0; j < nweights - i; j++) {
      int k = nweights - i - j;

      printf("%d\t%d\t%d\n", i, j, k);
    }
  }
}

int cmp_ints(const void *p1, const void *p2) {
  int a = *(int *)p1;
  int b = *(int *)p2;
  return b - a;
}

int main() {
  clock_t start_t = clock_time();

  char input[1024];
  read_input_file(input, 1024, "input_test.txt");
  int weights[32];
  int nweights = parse(input, weights);
  qsort(weights, nweights, sizeof(int), cmp_ints);
  
  for (int i = 0; i < nweights; i++) {
    printf("%d\n", weights[i]);
  }

  printf("parsed %d numbers\n", nweights);
  struct group groups[3] = {
      {.nweights = 0},
      {.nweights = 0},
      {.nweights = 0},
  };
  solve(weights, groups, nweights);

  int a1 = 0;
  int a2 = 0;
  printf("--- Day 24: It Hangs in the Balance ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}