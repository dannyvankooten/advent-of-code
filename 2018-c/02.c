#include "adventofcode.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PUZZLE_NAME "Day 2: Inventory Management System"

int pt1(const char *s) {
  uint8_t counts[26] = {0};
  int twos = 0;
  int threes = 0;

  while (*s != '\0') {
    if (*s == '\n') {
      // find twos and threes
      int c2 = 0;
      int c3 = 0;
      for (int i = 0; i < 26; i++) {
        if (counts[i] == 2) {
          c2 = 1;
        } else if (counts[i] == 3) {
          c3 = 1;
        }
      }

      memset(counts, 0, 26 * sizeof(uint8_t));
      twos += c2;
      threes += c3;
    } else {
      counts[(*s) - 'a']++;
    }

    s++;
  }

  return twos * threes;
}

void pt2(char *common, const char *s) {
  // 27 chars per line (incl. trailing \n)
  // 250 lines
  // 25 must match
  int ncommon = 0;
  for (int i = 0; i < 250; i++) {
    for (int j = i; j < 250; j++) {
      for (int k = 0; k < 26; k++) {
        if (s[i * 27 + k] == s[j * 27 + k]) {
          common[ncommon++] = s[j * 27 + k];
        }
      }

      if (ncommon == 25) {
        common[ncommon] = '\0';
        return;
      }

      ncommon = 0;
    }
  }
}

int main(void) {
  clock_t t = clock_time();
  char input[1024 * 64];
  read_input_file(input, 1024 * 64, "02.txt");

  int a1 = pt1(input);
  char buf[26];
  pt2(buf, input);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", a1);
  printf("Part 2: %s\n", buf);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
