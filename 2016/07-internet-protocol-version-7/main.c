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

int pt2(char *input) {
  int count = 0;
  int has_aba = 0;
  int has_bab = 0;
  char aba[2] = {0x0, 0x0};
  char bab[2] = {0x0, 0x0};
  int inside_square_brackets = 0;
  char *s = input;
  for(;;) {

    if (inside_square_brackets == 1) {
      // look for opposite of ABA
      if (aba[0] != 0x0) {
        if (*s == aba[1] && *(s+1) == aba[0] && *(s+2) == aba[1]) {
          has_bab = 1;
          s++;
          continue;
        }
      } else {
        // loop for BAB
        if (*s == *(s+2) && *(s+1) != *s) {
          has_bab = 1;
          bab[0] = *s;
          bab[1] = *(s+1);
          s++;
          continue;
        }
      }
    } else {
      // look FOR ABA
      if (bab[0] != 0) {
        if (*s == bab[1] && *(s+1) == bab[0] && *(s+2) == bab[1]) {
          has_aba = 1;
          s++;
          continue;
        }
      } else {
        // look for ABA
        if (*s == *(s+2) && *s != *(s+1)) {
          has_aba = 1;
          aba[0] = *s;
          aba[1] = *(s+1);
          s++;
          continue;
        }
      }
    }

    if (*s == '[') {
      inside_square_brackets = 1;
    } else if (*s == ']') {
      inside_square_brackets = 0;
    } else if (*s == '\n' || *s == 0x0) {
      if (has_aba == 1 && has_bab == 1) {
        count++;
      }
      has_aba = 0;
      aba[0] = 0x0;
      bab[0] = 0x0;
      has_bab = 0;
      inside_square_brackets = 0;
    }

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
