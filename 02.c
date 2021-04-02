#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int day2() {
  FILE* f = fopen("inputs/02.input", "r");
  if (!f) {
    err(EXIT_FAILURE, "error reading input file");
  }
  char buf[BUFSIZ];
  int32_t count_valid = 0;

  while ((fgets(buf, BUFSIZ, f) != NULL)) {
    char* s = buf;

    // read lower limit
    int32_t lower_limit = 0;
    while (*s >= '0' && *s <= '9') {
      lower_limit = (lower_limit * 10) + (*s - '0');
      s++;
    }

    // '-'
    s++;

    // read higher limit
    int32_t higher_limit = 0;
    while (*s >= '0' && *s <= '9') {
      higher_limit = (higher_limit * 10) + (*s - '0');
      s++;
    }

    // ' '
    s++;

    // read char requirement
    char c = *s++;

    // ": "
    s += 2;

    // read until "\n", check if c is between low and higher limit
    if ((s[lower_limit - 1] == c && s[higher_limit - 1] != c) ||
        (s[higher_limit - 1] == c && s[lower_limit - 1] != c)) {
      count_valid++;
    }
  }
  fclose(f);

  printf("%d\n", count_valid);
  assert(count_valid == 342);
  return 0;
}
