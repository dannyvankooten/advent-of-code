#include "../adventofcode.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  clock_t start_t = clock_time();

  char input[64 * 1024];
  read_input_file(input, 64 * 1024, "input.txt");

  const char *s = input;
  int pos[] = {0, 0};
  int dirs[][2] = {
      {0, 1},  // N
      {1, 0},  // E
      {0, -1}, // S
      {-1, 0}, // W
  };
  int cdir = 0; // N

  char dir;
  int amount;
  char seen[256 * 256] = {0};
  int pt2 = -1;

  while (*s != '\0') {
    dir = *s++;
    s = parse_int(&amount, s);

    // skip comma and space
    while (*s == ',' || *s == ' ' || *s == '\n')
      s++;

    if (dir == 'R') {
      cdir = (cdir == 3) ? 0 : cdir + 1;
    } else {
      cdir = (cdir == 0) ? 3 : cdir - 1;
    }

    while (amount > 0) {
      pos[0] += dirs[cdir][0];
      pos[1] += dirs[cdir][1];
      amount--;

      if (pt2 == -1) {
        int idx = (pos[0] + 128) * 256 + (pos[1] + 128);
        if (seen[idx] == 1) {
          pt2 = abs(pos[0]) + abs(pos[1]);
        }

        seen[idx] = 1;
      }
    }
  }

  printf("--- Day 1: No Time for a Taxicab ---\n");
  printf("Part 1: %d\n", abs(pos[0]) + abs(pos[1]));
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(start_t));
  return 0;
}
