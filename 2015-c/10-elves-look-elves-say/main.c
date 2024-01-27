#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int apply(char *dst, char *in) {
  char *s = in;
  char *o = dst;
  int d;

  while (*s != '\0') {
    d = 1;
    for (; s[d] == *s; d++)
      ;

    *o++ = (char)(d + '0');
    *o++ = *s;
    s += d;
  }

  *o = '\0';
  return (int)(o - dst);
}

int main(void) {
  clock_t start_t = clock_time();

  // single malloc call for 16MB of memory
  char *input = malloc_or_die(1024 * 1024 * 16);
  read_input_file(input, 64, "input.txt");

  char *buf = input + (1024 * 1024 * 8 * sizeof(char));
  char *tmp;

  int pt1;
  for (int i = 0; i < 40; i++) {
    pt1 = apply(buf, input);
    tmp = input;
    input = buf;
    buf = tmp;
  }

  int pt2;
  for (int i = 0; i < 10; i++) {
    pt2 = apply(buf, input);
    tmp = input;
    input = buf;
    buf = tmp;
  }

  printf("--- Day 10: Elves Look, Elves Say ---\n");
  printf("Part 1: %d %s\n", pt1, pt1 == 492982 ? "✔" : "");
  printf("Part 2: %d %s\n", pt2, pt2 == 6989950 ? "✔" : "");
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  free(input);
  return 0;
}
