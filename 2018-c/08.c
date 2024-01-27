#include "adventofcode.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PUZZLE_NAME "Day 8: Memory Maneuver"

const char *pt1(const char *s, int *sum) {
  int nchilds;
  int nmetadata;

  s = parse_int(&nchilds, s);
  s = skip(" ", s);
  s = parse_int(&nmetadata, s);
  s = skip(" ", s);

  for (int i = 0; i < nchilds; i++) {
    s = pt1(s, sum);
  }

  for (int i = 0; i < nmetadata; i++) {
    int value;
    s = parse_int(&value, s);
    if (*s == ' ')
      s++;
    *sum += value;
  }

  return s;
}

const char *pt2(const char *s, int *sum) {
  int nchilds;
  int nmetadata;

  s = parse_int(&nchilds, s);
  s = skip(" ", s);
  s = parse_int(&nmetadata, s);
  s = skip(" ", s);

  int child_values[nchilds];
  for (int i = 0; i < nchilds; i++) {
    child_values[i] = 0;
    s = pt2(s, &child_values[i]);
    // sum += child_values[i];
  }

  for (int i = 0; i < nmetadata; i++) {
    int value;
    s = parse_int(&value, s);
    if (*s == ' ')
      s++;

    if (nchilds == 0) {
      *sum += value;
    } else if (value > 0 && value <= nchilds) {
      *sum += child_values[value - 1];
    }
  }

  return s;
}

int main(void) {
  clock_t t = clock_time();
  char input[1024 * 64];
  read_input_file(input, 1024 * 64, "08.txt");

  int a1 = 0;
  pt1(input, &a1);

  int a2 = 0;
  pt2(input, &a2);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
