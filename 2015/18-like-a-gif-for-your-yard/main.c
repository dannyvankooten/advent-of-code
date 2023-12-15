#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int count_neighbors(const char *grid, int size, int y, int x) {
  static const char dirs[][2] = {
      {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},
  };

  int count = 0;
  for (int n = 0; n < 8; n++) {
    int ny = y + dirs[n][0];
    int nx = x + dirs[n][1];

    if (nx < 0 || nx >= size || ny < 0 || ny >= size) {
      continue;
    }

    if (grid[ny * size + nx] == '#') {
      count++;
    }
  }

  return count;
}

int count_on(const char *grid, int size) {
  int count = 0;
  for (int i = 0; i < size * size; i++) {
    if (grid[i] == '#') {
      count++;
    }
  }

  return count;
}

void mutate(char *grid, int size, int broken) {
  char buf[size * size];
  memcpy(buf, grid, size * size * sizeof(char));

  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      int c = count_neighbors(buf, size, y, x);
      if (buf[y * size + x] == '#') {
        grid[y * size + x] = c == 2 || c == 3 ? '#' : '.';
      } else {
        grid[y * size + x] = c == 3 ? '#' : '.';
      }

      // keep corners turned on
      if (broken == 1) {
        grid[0] = '#';
        grid[size - 1] = '#';
        grid[(size - 1) * size] = '#';
        grid[(size - 1) * size + size - 1] = '#';
      }
    }
  }
}

int solve(char *g, int size, int broken) {
  char grid[size * size];
  memcpy(grid, g, size * size * sizeof(char));

  if (broken == 1) {
    grid[0] = '#';
    grid[size - 1] = '#';
    grid[(size - 1) * size] = '#';
    grid[(size - 1) * size + size - 1] = '#';
  }

  for (int steps = 1; steps <= 100; steps++) {
    mutate(grid, size, broken);
  }

  return count_on(grid, size);
}

int main() {
  clock_t start_t = clock_time();
  char input[1024 * 32] = "";
  read_input_file(input, 32 * 1024, "input.txt");

  const int size = 100;
  char grid[size * size];
  char *s = input;
  char *g = grid;
  while (*s != '\0') {
    while (*s != '\n' && *s != '\0') {
      *g++ = *s++;
    }

    if (*s == '\n')
      s++;
  }

  int a1 = solve(grid, size, 0);
  int a2 = solve(grid, size, 1);

  printf("--- Day 18: Like a GIF For Your Yard ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", clock_time_since(start_t));
  return 0;
}