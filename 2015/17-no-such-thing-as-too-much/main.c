#include "../adventofcode.h"
#include <stdio.h>
#include <time.h>

int min_containers = 1 << 30;
int pt2 = 0;

int permute(int sizes[], int flags, int i, int n) {
  if (i == n) {
    int s = 0;
    int c = 0;
    int b;
    for (int x = 0; x < n; x++) {
      b = (int)((flags >> (n - x)) & 1);
      c += b;
      s += sizes[x] * b;
    }

    if (s == 150) {
      if (c < min_containers) {
        min_containers = c;
        pt2 = 1;
      } else if (c == min_containers) {
        pt2++;
      }

      return 1;
    }
    return 0;
  }

  // try to bail early
  int s = 0;
  for (int x = 0; x < i; x++) {
    s += sizes[x] * (int)((flags >> (n - x)) & 1);
  }
  if (s > 150) {
    return 0;
  }

  return permute(sizes, flags, i + 1, n) +
         permute(sizes, flags | 1 << (n - i), i + 1, n);
}

int qsort_compare_int_desc(const void *ptra, const void *ptrb) {
  int a = *(int *)ptra;
  int b = *(int *)ptrb;
  return b - a;
}

int parse(const char *s, int *dest) {
  int n = 0;
  while (*s != 0x0) {
    while (*s == ' ') {
      s++;
    }
    s = parse_int(&dest[n++], s);
    s = skip_optional('\n', s);
  }

  qsort(dest, (size_t)n, sizeof(*dest), qsort_compare_int_desc);
  return n;
}

int main() {
  clock_t start_t = clock_time();

  char input[1024];
  read_input_file(input, 1024, "input.txt");
  int sizes[32];
  int n = parse(input, sizes);
  int flags = 0;
  int count = permute(sizes, flags, 0, n);

  printf("--- Day 17: No Such Thing as Too Much ---\n");
  printf("Part 1: %d\n", count);
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(start_t));
  return 0;
}