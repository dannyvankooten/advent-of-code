#include "../adventofcode.h"
#include <stdio.h>

int main(void) {
  clock_t start_t = clock_time();
  char input[32 * 1024];
  read_input_file(input, 32 * 1024, "input.txt");

  char *s = input;
  int pt1 = 0;
  int pt2 = 0;

  while (*s != '\0') {
    // skip opening "
    s++;

    // opening and closing ""
    pt1 += 2;
    pt2 += 4;

    while (*s != '"') {
      if (*s == '\\') {
        // hexadecimal ASCII
        if (*(s + 1) == 'x') {
          s += 4;
          pt1 += 3;
          pt2 += 1;
        } else {
          // single escaped character
          s += 2;
          pt1 += 1;
          pt2 += 2;
        }

        continue;
      }

      // normal character
      s++;
    }

    // skip closing "
    s++;

    // skip newline
    s++;
  }

  printf("--- Day 8: Matchsticks ---\n");
  printf("Part 1: %d %s\n", pt1, pt1 == 1342 ? "✔" : "");
  printf("Part 2: %d %s\n", pt2, pt2 == 2074 ? "✔" : "");
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  return 0;
}
