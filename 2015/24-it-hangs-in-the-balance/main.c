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

// qsort callback
int cmp_ints(const void *p1, const void *p2) {
  int a = *(int *)p1;
  int b = *(int *)p2;
  return b - a;
}

int main() {
  clock_t start_t = clock_time();

  char input[1024];
  read_input_file(input, 1024, "input.txt");
  int weights[32];
  int nweights = parse(input, weights);
  qsort(weights, nweights, sizeof(int), cmp_ints);
  int weights_sum = 0;
  for (int i = 0; i < nweights; i++) {
    weights_sum += weights[i];
  }

  int group_sum = weights_sum / 3;
  uint64_t qe = 1;
  for (int s = 0, i = 0; i < nweights && s < group_sum; i++) {
    if (s + weights[i] < group_sum) {
      s += weights[i];
      qe *= weights[i];
    }
  }

  int a2 = 0;
  printf("--- Day 24: It Hangs in the Balance ---\n");
  printf("Part 1: %ld\n", qe);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}
