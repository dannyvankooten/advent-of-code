#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int day1() {
  FILE* f = fopen("inputs/01.input", "r");
  if (!f) {
    err(EXIT_FAILURE, "error reading input file");
  }
  char linebuf[BUFSIZ];
  int32_t numbers[200] = {0};
  int32_t n = 0;
  while ((fgets(linebuf, BUFSIZ, f) != NULL)) {
    char* s = linebuf;
    while (*s >= '0' && *s <= '9') {
      numbers[n] = (numbers[n] * 10) + (*s - '0');
      s++;
    }
    n++;
  }
  fclose(f);

  for (int32_t i = 0; i < n; i++) {
    for (int32_t j = i; j < n; j++) {
      for (int32_t k = j; k < n; k++) {
        if (numbers[i] + numbers[j] + numbers[k] == 2020) {
          printf("%d\n", numbers[i] * numbers[j] * numbers[k]);
          return 0;
        }
      }
    }
  }
  return 0;
}