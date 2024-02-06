#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  int8_t x;
  int8_t y;
} pos_t;

static int solve(const char *s, const uint8_t nworkers) {
  int8_t dx, dy;
  uint16_t tally[256 * 256] = {0};
  uint8_t w = 0;
  pos_t *workers = malloc(nworkers * sizeof(pos_t));
  if (workers == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }
  for (uint8_t i = 0; i < nworkers; i++) {
    workers[i].x = 0;
    workers[i].y = 0;
  }

  // all workers deliver at root address
  tally[(0 + 128) * 256 + 128] = nworkers;

  while (*s != '\0') {

    switch (*s) {
    case '^':
      dx = 0;
      dy = -1;
      break;
    case '>':
      dx = 1;
      dy = 0;
      break;

    case 'v':
      dx = 0;
      dy = 1;
      break;

    case '<':
      dx = -1;
      dy = 0;
      break;

    default:
      s++;
      continue;
      break;
    }

    workers[w].x += dx;
    workers[w].y += dy;
    tally[(workers[w].y + 128) * 256 + (workers[w].x + 128)]++;

    // next iteration is for next worker
    w++;
    if (w >= nworkers) {
      w = 0;
    }

    // move to next char in input
    s++;
  }

  uint16_t count = 0;
  for (uint32_t i = 0; i < 256 * 256; i++) {
    if (tally[i] > 0) {
      count += 1;
    }
  }

  free(workers);
  return count;
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

  printf("--- Day 3: Perfectly Spherical Houses in a Vacuum ---\n");
  printf("Part 1: %d\n", solve(input, 1));
  printf("Part 2: %d\n", solve(input, 2));

  end_t = clock();
  double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
  printf("Time: %.2f ms\n", total_t);
  return 0;
}
