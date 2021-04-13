#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputs/09.h"

int day9() {
  const unsigned char *s = input;
  int32_t numbers[1000];
  uint32_t numbers_n = 0;
  int32_t invalid_n = 104054607;
  while (*s != '\0') {
    int32_t n = 0;
    while (*s >= '0' && *s <= '9') {
      n = n * 10 + (*s - '0');
      s++;
    }
    numbers[numbers_n++] = n;

    if (*s == '\n') s++;
  }

  // loop through numbers to find contiguous set
  // that sums to invalid_n (127)
  uint32_t range_start = 0;
  uint32_t range_end = 0;
  for (uint32_t i = 0; i < numbers_n - 1; i++) {
    int32_t sum = numbers[i];
    uint32_t j = i;
    for (; j < numbers_n - 1 && sum < invalid_n;) {
      sum += numbers[++j];
    }

    if (sum == invalid_n && j > i) {
      range_start = i;
      range_end = j;
      break;
    }
  }

  int32_t smallest = numbers[range_start];
  int32_t largest = numbers[range_start];
  for (uint32_t i = range_start; i <= range_end; i++) {
    if (numbers[i] < smallest) {
      smallest = numbers[i];
    } else if (numbers[i] > largest) {
      largest = numbers[i];
    }
  }

  int32_t answer = largest + smallest;
  printf("%d\n", answer);
  assert(answer == 13935797);

  return EXIT_SUCCESS;
}
