#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../adventofcode.h"

#define PUZZLE_NAME "Day 7: Internet Protocol Version 7"

// TODO: We store only one ABA / BAB per line here
// But what we need to do is whenever we encounter an ABA, search forward for a matching BAB inside square brackets and then bail


int pt1(char *input) {
  int count = 0;
  int has_abba_outside = 0;
  int has_abba_inside = 0;
  int inside_square_brackets = 0;
  char *s = input;
  for(;;) {
    if (
      *s != 0x0
      && *(s+1) != 0x0
      && *(s+2) != 0x0
      && *(s+3) != 0x0
      && *s != *(s+1)
      && *s == *(s+3)
      && *(s+1) == *(s+2)) {
        if (inside_square_brackets == 1) {
          has_abba_inside = 1;
        } else {
          has_abba_outside = 1;
        }
        s += 4;
        continue;
      } else if (*s == '[') {
      inside_square_brackets = 1;
    } else if (*s == ']') {
      inside_square_brackets = 0;
    } else if (*s == '\n' || *s == 0x0) {
      if (has_abba_outside == 1 && has_abba_inside == 0) {
        count++;
      }
      has_abba_inside = 0;
      has_abba_outside = 0;
      inside_square_brackets = 0;
    }

    if (*s++ == 0x0) break;
  }

  return count;
}

static int has_bab(const char *line, char a, char b) {
  const char *s = line;

  while (*s != '\n' && *s != 0x0) {
    while (*s != '[' && *s != '\n' && *s != 0x0) s++;
    if (*s == '\n' || *s == 0x0) { break; }
    while (*s != ']') {
        if (*s == b && *(s+2) == b && *(s+1) == a) {
          return 1;
        }
        s++;
    }

  }

  return 0;
}

int pt2(const char *input) {
  int count = 0;
  const char *line_start;
  const char *s = input;
  char a, b;
  for(;;) {
    line_start = s;

    while (*s != '\n' && *s != 0x0) {
      // skip parts inside square brackets
      if (*s == '[') {
        while (*s != ']') s++;
      }

      // check for ABA
      if (*s == *(s+2) && *s != *(s+1)) {
        a = *s;
        b = *(s+1);

        // search back through entire line for matching BAB
        if (has_bab(line_start, a, b) == 1) {
          count++;
          while (*s != '\n' && *s != 0x0) s++;
          break;
        }
      }

      s++;
    }

    // break if we reached EOF
    // continue if \n
    if (*s++ == 0x0) break;
  }

  return count;
}


int main() {
  clock_t t = timer_start();
  char *input = calloc(256 * 1024, sizeof(char));
  if (input == NULL) {
    return EXIT_FAILURE;
  }
  read_input_file(input, 256*1024, "input.txt");

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", pt1(input));
  printf("Part 2: %d\n", pt2(input));
  printf("Time: %.2fms\n", timer_stop(t));
  free(input);
  return EXIT_SUCCESS;
}
