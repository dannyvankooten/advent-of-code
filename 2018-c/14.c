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

  m = digits;
  do {
    buf[--m] = ((char)(n % 10));
    n /= 10;
  } while (m != 0);

  return digits;
}

static void test_itos(void) {
  char buf[32];
  itos(1, buf);
  assert(memcmp(buf, (char[]){1}, 1) == 0);
  itos(20, buf);
  assert(memcmp(buf, (char[]){2, 0}, 2) == 0);
  itos(22, buf);
  assert(memcmp(buf, (char[]){2, 2}, 2) == 0);
}

int main(void) {
  test_itos();

  clock_t t = clock_time();

  char line[1024];
  fgets(line, 1024, stdin);
  unsigned n = (unsigned)atoi(line);

  char needle[32];
  sprintf(needle, "%d", n);
  const unsigned long needle_len = strlen(needle);
  for (unsigned i = 0; i < needle_len; i++) {
    needle[i] -= '0';
  }

  char *recipes = malloc((unsigned long)(n + 10) * 40ul * sizeof(char));
  assert(recipes != NULL);

  unsigned nrecipes = 2;
  recipes[0] = 3;
  recipes[1] = 7;

  unsigned elf_a = 0;
  unsigned elf_b = 1;
  unsigned pt2 = 0;
  char buf[32];

  while (1) {
    unsigned a = (unsigned)recipes[elf_a];
    unsigned b = (unsigned)recipes[elf_b];
    unsigned r = a + b;

    unsigned digits = itos(r, buf);
    memcpy(&recipes[nrecipes], buf, digits * sizeof(char));
    nrecipes += digits;

    elf_a = (elf_a + 1 + a) % nrecipes;
    elf_b = (elf_b + 1 + b) % nrecipes;

    pt2 = (unsigned)(nrecipes - needle_len - 1);
    if (nrecipes > needle_len &&
        memcmp(&recipes[pt2], needle, needle_len) == 0) {
      break;
    }
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
