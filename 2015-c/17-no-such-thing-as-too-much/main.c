#include "../adventofcode.h"
#include <stdio.h>

unsigned int min_containers = 1 << 30;
int pt2 = 0;

unsigned int permute(unsigned int sizes[],unsigned int flags, unsigned int i, unsigned int n) {
  if (i == n) {
    int s = 0;
    unsigned int c = 0;
    unsigned int b;
    for (unsigned int x = 0; x < n; x++) {
      b = (unsigned int)((flags >> (n - x)) & 1);
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
  unsigned int s = 0;
  for (unsigned int x = 0; x < i; x++) {
    s += sizes[x] * (unsigned int)((flags >> (n - x)) & 1);
  }
  if (s > 150) {
    return 0;
  }

  return permute(sizes, flags, i + 1, n) +
         permute(sizes, flags | 1 << (n - i), i + 1, n);
}

static int qsort_compare_int_desc(const void *ptra, const void *ptrb) {
  const unsigned int a = *(unsigned int *)ptra;
  const unsigned int b = *(unsigned int *)ptrb;
  return (b > a) - (b < a);
}

unsigned int parse(const char *s,unsigned int *dest) {
  unsigned int n = 0;
  while (*s != 0x0) {
    while (*s == ' ') {
      s++;
    }
    s = parse_uint(&dest[n++], s);
    s = skip_optional('\n', s);
  }

  qsort(dest, (size_t)n, sizeof(*dest), qsort_compare_int_desc);
  return n;
}

int main(void) {
  clock_t start_t = clock_time();

  char input[1024];
  read_input_file(input, 1024, "input.txt");
  unsigned int sizes[32];
  unsigned int n = parse(input, sizes);
  unsigned int flags = 0;
  unsigned int count = permute(sizes, flags, 0,(unsigned) n);

  printf("--- Day 17: No Such Thing as Too Much ---\n");
  printf("Part 1: %d\n", count);
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  return 0;
}
