#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static inline char *parse_int(uint16_t *dst, char *s) {
  int n = 0;
  while (*s >= '0' && *s <= '9') {
    n = (n * 10) + (*s - '0');
    s++;
  }

  *dst = (uint16_t)n;
  return s;
}

int main(void) {
  clock_t start_t, end_t;
  start_t = clock();

  FILE *fp = fopen("input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "error reading input.txt");
    exit(EXIT_FAILURE);
  }
  char input[32 * 1024] = "";
  size_t nread = fread(input, 1, 32 * 1024, fp);
  input[nread] = '\0';
  fclose(fp);

  uint8_t *grid_pt1 = calloc(1000 * 1000, sizeof(uint8_t));
  if (grid_pt1 == NULL) {
    exit(EXIT_FAILURE);
  }

  uint16_t *grid_pt2 = calloc(1000 * 1000, sizeof(uint16_t));
  if (grid_pt2 == NULL) {
    exit(EXIT_FAILURE);
  }

  int8_t action;
  uint16_t x_start;
  uint16_t y_start;
  uint16_t x_end;
  uint16_t y_end;

  char *s = input;
  while (*s != '\0') {
    if (strncmp(s, "toggle", 6) == 0) {
      action = -1;
      s += 7;
    } else if (strncmp(s + 5, "on", 2) == 0) {
      action = 1;
      s += 8;
    } else if (strncmp(s + 5, "off", 3) == 0) {
      action = 0;
      s += 9;
    } else {
      printf("unexpected input at %s\n", s);
      abort();
    }

    // parse start coords
    s = parse_int(&x_start, s);
    s++; // skip comma
    s = parse_int(&y_start, s);

    // skip " through "
    s += 9;

    // parse end coords
    s = parse_int(&x_end, s);
    s++; // skip comma
    s = parse_int(&y_end, s);

    // skip newline
    s++;

    // update grids
    for (uint16_t y = y_start; y <= y_end; y++) {
      for (uint16_t x = x_start; x <= x_end; x++) {
        int idx = y * 1000 + x;
        switch (action) {
        case 0:
          grid_pt1[idx] = 0;
          if (grid_pt2[idx] > 0) {
            grid_pt2[idx]--;
          }
          break;
        case 1:
          grid_pt1[idx] = 1;
          grid_pt2[idx] += 1;
          break;

        case -1:
          grid_pt1[idx] = grid_pt1[idx] == 1 ? 0 : 1;
          grid_pt2[idx] += 2;
          break;
        }
      }
    }
  }

  printf("--- Day 6: Probably a Fire Hazard ---\n");
  // we loop separately here because it's faster (because of cpu cache hits)
  int pt1 = 0;
  for (int i = 0; i < 1000 * 1000; i++) {
    if (grid_pt1[i] == 1) {
      pt1++;
    }
  }
  printf("Part 1: %d\n", pt1);

  long pt2 = 0;
  for (int i = 0; i < 1000 * 1000; i++) {
    pt2 += grid_pt2[i];
  }
  printf("Part 2: %ld\n", pt2);

  end_t = clock();
  double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
  printf("Time: %.2f ms\n", total_t);

  free(grid_pt1);
  free(grid_pt2);
  return 0;
}
