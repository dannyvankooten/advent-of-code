#include "adventofcode.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 10: Knot Hash"

static unsigned int itos(unsigned n, char buf[static 32]) {
  unsigned m = 10;
  unsigned digits = 1;
  while (m <= n) {
    m *= 10;
    digits++;
  }

  assert(digits < 31);
  assert(digits > 0);

  m = digits;
  buf[m] = '\0';
  while (1) {
    buf[--m] = ((char) (n % 10) + '0');

    if (n < 10) {
      break;
    }

    n /= 10;
  }

  return digits;
}

int main(void) {
  char buf[32];

  itos(1, buf);
  assert(memcmp(buf, "1", 1) == 0);
  itos(20, buf);
  assert(memcmp(buf, "20", 2) == 0);
  itos(22, buf);
  assert(memcmp(buf, "22", 2) == 0);

  clock_t t = clock_time();

  char line[1024];
  fgets(line, 1024, stdin);
  unsigned n = (unsigned)atoi(line);

  char needle[32];
  sprintf(needle, "%d", n);
  unsigned long needle_len = strlen(needle);
  for (unsigned i = 0; i < needle_len; i++) {
    needle[i] -= '0';
  }

  char *recipes = malloc_or_die((unsigned long)(n + 10) * 40ul * sizeof(char));
  unsigned nrecipes = 2;
  recipes[0] = 3;
  recipes[1] = 7;

  unsigned elf_a = 0;
  unsigned elf_b = 1;
  unsigned pt2 = 0;

  while (1) {
    unsigned a = (unsigned)recipes[elf_a];
    unsigned b = (unsigned)recipes[elf_b];
    unsigned r = a + b;

    unsigned long digits = itos(r, buf);
    for (unsigned i = 0; i < digits; i++) {
      recipes[nrecipes++] = buf[i] - '0';
    }

    pt2 = (unsigned) (nrecipes - needle_len - 1);
    if (recipes[nrecipes - 1] == needle[needle_len - 1] &&
        memcmp(&recipes[pt2], needle, needle_len) == 0) {
      break;
    }

    elf_a = (elf_a + (1u + a)) % nrecipes;
    elf_b = (elf_b + (1u + b)) % nrecipes;
  }

  unsigned pt1 = 0;
  for (unsigned i = 0; i < 10; i++) {
    pt1 = (pt1 * 10) + (unsigned)recipes[n + i];
  }

  free(recipes);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", pt1);
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
