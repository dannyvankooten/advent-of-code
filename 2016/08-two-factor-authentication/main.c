#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 8: Two Factor Authentication"

void shift_row(char grid[6][50], int row, int n) {
  char copy[50];
  memcpy(copy, grid[row], 50 * sizeof(char));

  for (int x = 0; x < 50; x++) {
    int src = x - n;
    if (src < 0)
      src += 50;
    grid[row][x] = copy[src];
  }
}

void shift_column(char grid[6][50], int col, int n) {
  char copy[6][50];
  memcpy(copy, grid, 6 * 50 * sizeof(char));

  for (int y = 0; y < 6; y++) {
    int src = y - n;
    if (src < 0)
      src += 6;
    grid[y][col] = copy[src][col];
  }
}

int main() {
  clock_t t = clock_time();
  char input[1024 * 64];
  read_input_file(input, 1024 * 64, "input.txt");

  char grid[6][50];
  memset(grid, '.', 6 * 50 * sizeof(char));

  const char *s = input;
  char dim[32];
  int axes;
  int width;
  int height;
  int amount;
  while (*s != 0x0) {
    if (*(s + 1) == 'o') {
      // rotate row y=0 by 2
      s = skip("rotate ", s);
      s = parse_ident(dim, s);
      s = skip(" ", s);
      s += 2;
      s = parse_int(&axes, s);
      s = skip(" by ", s);
      s = parse_int(&amount, s);

      if (strcmp(dim, "column") == 0) {
        shift_column(grid, axes, amount);
      } else {
        shift_row(grid, axes, amount);
      }
    } else {
      // rect 1x2
      sscanf(s, "rect %dx%d", &width, &height);

      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          grid[y][x] = '#';
        }
      }
    }

    while (*s != '\n' && *s != 0x0)
      s++;
    if (*s == '\n')
      s++;
  }

  int pt1 = 0;
  for (int y = 0; y < 6; y++) {
    for (int x = 0; x < 50; x++) {
      pt1 += grid[y][x] == '#' ? 1 : 0;
    }
  }

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", pt1);
  printf("Part 2: \n");
  for (int y = 0; y < 6; y++) {
    for (int x = 0; x < 50; x++) {
      if (x > 0 && x % 5 == 0)
        printf("  ");
      printf("%c", grid[y][x] == '#' ? '@' : ' ');
    }
    printf("\n");
  }

  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
