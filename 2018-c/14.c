#include "adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 10: Knot Hash"

int main(void) {
  clock_t t = clock_time();

  char line[1024];
  fgets(line, 1024, stdin);
  unsigned n = (unsigned) atoi(line);

  char *recipes = malloc_or_die((unsigned long)(n+10) * 40ul * sizeof(char));
  unsigned nrecipes = 2;
  recipes[0] = 3;
  recipes[1] = 7;

  unsigned elf_a = 0;
  unsigned elf_b = 1;
  char buf[32];
  while (nrecipes < (n+10) * 40ul) {
    unsigned a = (unsigned) recipes[elf_a];
    unsigned b = (unsigned) recipes[elf_b];
    unsigned r = a + b;

    sprintf(buf, "%d", r);
    unsigned long len = strlen(buf);
    for (unsigned i = 0; i < len && nrecipes < (n+10) * 40ul; i++) {
      recipes[nrecipes++] = buf[i] - '0';
    }

    elf_a = (elf_a + (1u + a)) % nrecipes;
    elf_b = (elf_b + (1u + b)) % nrecipes;
  }

  unsigned pt1 = 0;
  for (unsigned i = 0; i < 10; i++) {
   pt1 = (pt1*10) + (unsigned) recipes[n+i];
  }


  char needle[32];
  sprintf(needle, "%d", n);
  unsigned long needle_len = strlen(needle);
  for (unsigned i = 0; i < needle_len; i++) {
    needle[i] -= '0';
  }

  unsigned pt2 = 0;
  char *p = recipes;
  while (1) {
    // TODO: Potentially speed this up by jumping to next memchr result
    // OR, use strstr if we resort to string digits
    if (memcmp(p, needle, needle_len) == 0) {
      pt2 = (unsigned) (p - recipes);
      break;
    }

    p++;
  }

  free(recipes);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", pt1);
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
