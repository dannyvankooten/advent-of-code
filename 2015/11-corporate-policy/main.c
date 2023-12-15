#include "../adventofcode.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int pw_is_valid(char *pw) {
  char *s = pw;

  uint_fast16_t pairs = 0;
  uint_fast16_t straights = 0;
  uint_fast16_t i = 0;
  while (*s != '\0') {

    switch (*s) {
    case 'i':
    case 'o':
    case 'l':
      return false;
    }

    // check for non-overlapping pair
    if (*s == *(s + 1) && (i == 0 || *(s - 1) != *s)) {
      pairs++;
    } else if (*(s + 1) == (*s) + 1 && *(s + 2) == (*s) + 2) {
      straights++;
    }

    if (*s == 'i' || *s == 'o' || *s == 'l') {
      return false;
    }

    s++;
    i++;
  }

  return pairs >= 2 && straights >= 1;
}

void pw_increment(char *src) {
  do {
    int_fast8_t len = strlen(src);
    for (int_fast8_t i = len - 1; i >= 1; i--) {
      if (src[i] == 'z') {
        src[i] = 'a';
      } else {
        src[i]++;
        break;
      }
    }
  } while (pw_is_valid(src) == 0);
}

int main() {
  clock_t start_t = clock_time();

  assert(pw_is_valid("hijklmmn") == 0);
  assert(pw_is_valid("abbceffg") == 0);
  assert(pw_is_valid("abbcegjk") == 0);
  assert(pw_is_valid("abcdffaa") == 1);
  assert(pw_is_valid("ghjaabcc") == 1);

  char buf[16];
  read_input_file(buf, 16, "input.txt");

  printf("--- Day 11: Corporate Policy ---\n");
  pw_increment(buf);
  printf("Part 1: %s\n", buf);
  assert(strcmp(buf, "cqjxxyzz") == 0);

  pw_increment(buf);
  printf("Part 2: %s\n", buf);
  assert(strcmp(buf, "cqkaabcc") == 0);

  printf("Time: %.2fms\n", clock_time_since(start_t));
  return 0;
}