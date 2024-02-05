#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static inline char *parse(int *dst, char *s) {
  int d = 0;

  while (*s >= '0' && *s <= '9') {
    d = (d * 10) + (*s - '0');
    s++;
  }

  *dst = d;
  return s;
}

int compare_ints(const void *ptr_a, const void *ptr_b) {
  const int a = *(const int *)ptr_a;
  const int b = *(const int *)ptr_b;
  return (a > b) - (a < b);
}

int main(void) {
  clock_t start_t, end_t;
  start_t = clock();

  FILE *fp = fopen("input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "error reading input.txt");
    exit(EXIT_FAILURE);
  }
  char input[64 * 1024];
  size_t nread = fread(input, 1, 64 * 1024, fp);
  input[nread] = '\0';
  fclose(fp);

  char *s = input;
  int area = 0;
  int ribbon = 0;
  int dimensions[3];

  while (*s != '\0') {
    s = parse(&dimensions[0], s);
    s++;
    s = parse(&dimensions[1], s);
    s++;
    s = parse(&dimensions[2], s);
    s++;

    qsort(dimensions, 3, sizeof(int), compare_ints);

    area += 2 * dimensions[0] * dimensions[1] +
            2 * dimensions[1] * dimensions[2] +
            2 * dimensions[0] * dimensions[2] + dimensions[0] * dimensions[1];
    ribbon += 2 * dimensions[0] + 2 * dimensions[1] +
              dimensions[0] * dimensions[1] * dimensions[2];
  }

  printf("--- Day 2: I Was Told There Would Be No Math ---\n");
  fprintf(stdout, "Part 1: %d\n", area);
  fprintf(stdout, "Part 2: %d\n", ribbon);

  end_t = clock();
  double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
  printf("Time: %.2f ms\n", total_t);
  return 0;
}
