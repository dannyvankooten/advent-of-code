#include "../adventofcode.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int pt1(char *input) {
  int n = 0;
  char ident[32];
  int amount;

  const char *s = input;
  while (*s != '\0') {
    while (*s != ':')
      s++;
    s++;

    while (*s != '\n') {
      s = parse_ident(ident, s);
      s += 2; // ": "
      s = parse_int(&amount, s);
      if (*s == ',')
        s += 2;

      if (strcmp(ident, "children") == 0 && amount != 3) {
        goto skip;
      } else if (strcmp(ident, "cats") == 0 && amount != 7) {
        goto skip;
      } else if (strcmp(ident, "samoyeds") == 0 && amount != 2) {
        goto skip;
      } else if (strcmp(ident, "pomeranians") == 0 && amount != 3) {
        goto skip;
      } else if (strcmp(ident, "akitas") == 0 && amount != 0) {
        goto skip;
      } else if (strcmp(ident, "vizslas") == 0 && amount != 0) {
        goto skip;
      } else if (strcmp(ident, "goldfish") == 0 && amount != 5) {
        goto skip;
      } else if (strcmp(ident, "trees") == 0 && amount != 3) {
        goto skip;
      } else if (strcmp(ident, "cars") == 0 && amount != 2) {
        goto skip;
      } else if (strcmp(ident, "perfumes") == 0 && amount != 1) {
        goto skip;
      }
    }

    return n + 1;

  skip:
    while (*s != '\n' && *s != '\0')
      s++;
    if (*s == '\n')
      s++;
    n++;
  }

  return -1;
}

int pt2(char *input) {
  int n = 0;
  char ident[32];
  int amount;

  const char *s = input;
  while (*s != '\0') {
    while (*s != ':')
      s++;
    s += 2; // ": "

    while (*s != '\n') {
      s = parse_ident(ident, s);
      if (*s != ':')
        abort();
      s += 2; // ": "
      s = parse_int(&amount, s);
      if (*s == ',')
        s += 2; // ", "

      if ((strcmp(ident, "children") == 0 && amount != 3) ||
          (strcmp(ident, "cats") == 0 && amount <= 7) ||
          (strcmp(ident, "samoyeds") == 0 && amount != 2) ||
          (strcmp(ident, "pomeranians") == 0 && amount >= 3) ||
          (strcmp(ident, "akitas") == 0 && amount != 0) ||
          (strcmp(ident, "vizslas") == 0 && amount != 0) ||
          (strcmp(ident, "goldfish") == 0 && amount >= 5) ||
          (strcmp(ident, "trees") == 0 && amount <= 3) ||
          (strcmp(ident, "cars") == 0 && amount != 2) ||
          (strcmp(ident, "perfumes") == 0 && amount != 1)) {
        goto skip;
      }
    }

    return n + 1;
  skip:

    while (*s != '\n' && *s != '\0')
      s++;
    if (*s == '\n')
      s++;
    n++;
  }

  return -1;
}

int main() {
  clock_t start_t = clock_time();
  char input[64 * 1024];
  read_input_file(input, 64 * 1024, "input.txt");

  printf("--- Day 16: Aunt Sue ---\n");
  int a1 = pt1(input);
  printf("Part 1: %d\n", a1);
  assert(a1 == 213);

  int a2 = pt2(input);
  printf("Part 2: %d\n", a2);
  assert(a2 == 323);

  printf("Time: %.2fms\n", clock_time_since(start_t));
  return 0;
}