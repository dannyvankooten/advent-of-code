#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "inputs/01.h"

static int
intcmp(const void *p1, const void *p2) {
  int a = *(int *) p1;
  int b = *(int *) p2;
  if (a == b) {
    return 0;
  }

  return a < b ? -1 : 1;
}

int day1(void) {
  int numbers[200] = {0};
  int n = 0;
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
  qsort(numbers, n, sizeof(int), intcmp);

  int answer = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
      for (int k = j; k < n; k++) {
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
