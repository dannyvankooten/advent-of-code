#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputs/09.h"

int day9() {
  const unsigned char *s = input;
  int64_t numbers[1000];
  size_t numbers_n = 0;
  int64_t invalid_n = 104054607;
  while (*s != '\0') {
    int64_t n = 0;
    while (*s >= '0' && *s <= '9') {
      n = n * 10 + (*s - '0');
      s++;
    }
    numbers[numbers_n++] = n;

    if (*s == '\n') s++;
  }

  // loop through numbers to find contiguous set
  // that sums to invalid_n (127)
  size_t range_start = 0;
  size_t range_end = 0;
  for (size_t i = 0; i < numbers_n - 1; i++) {
    int64_t sum = numbers[i];
    size_t j = i;
    for (; j < numbers_n - 1 && sum < invalid_n;) {
      sum += numbers[++j];
    }

    if (sum == invalid_n && j > i) {
      range_start = i;
      range_end = j;
      break;
    }
  }

  int64_t smallest = numbers[range_start];
  int64_t largest = numbers[range_start];
  for (size_t i = range_start; i <= range_end; i++) {
    if (numbers[i] < smallest) {
      smallest = numbers[i];
    } else if (numbers[i] > largest) {
      largest = numbers[i];
    }
  }

  int64_t answer = largest + smallest;
  printf("%ld\n", answer);
  assert(answer == 13935797);

  return EXIT_SUCCESS;
}
