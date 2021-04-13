#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "inputs/01.h"

static int 
intcmp(const void *p1, const void *p2) {
  int32_t a = *(int32_t *) p1;
  int32_t b = *(int32_t *) p2;
  if (a == b) {
    return 0;
  }

  return a < b ? -1 : 1;
}

int day1() {
  int32_t numbers[200] = {0};
  int32_t n = 0;
  const unsigned char *s = input;
  while (*s != '\0') {
    while (*s >= '0' && *s <= '9') {
      numbers[n] = (numbers[n] * 10) + (*s - '0');
      s++;
    }

    if (*s == '\n') s++;
    n++;
  }

  // Since the 3 numbers need to sum to 2020 and most input numbers are over half that
  // We're likely to find out answer sooner on a sorted list (small to large).
  qsort(numbers, n, sizeof(int32_t), intcmp);

  int32_t answer = 0;
  for (int32_t i = 0; i < n; i++) {
    for (int32_t j = i; j < n; j++) {
      for (int32_t k = j; k < n; k++) {
        if (numbers[i] + numbers[j] + numbers[k] == 2020) {
          answer = numbers[i] * numbers[j] * numbers[k];
          goto PRINT_RESULT;
        }
      }
    }
  }

PRINT_RESULT: ;
  printf("%d\n", answer);
  assert(answer == 157667328);
  return 0;
}