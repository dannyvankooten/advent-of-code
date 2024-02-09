#include "inputs/09.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int day9(void) {
  const unsigned char *s = input;
  unsigned int numbers[1000];
  unsigned int numbers_n = 0;
  unsigned int invalid_n = 104054607;
  while (*s != '\0') {
    unsigned int n = 0;
    while (*s >= '0' && *s <= '9') {
      n = n * 10ul + (*s - '0');
      s++;
    }
    numbers[numbers_n++] = n;

    if (*s == '\n')
      s++;
  }

  // loop through numbers to find contiguous set
  // that sums to invalid_n (127)
  unsigned int range_start = 0;
  unsigned int range_end = 0;
  for (unsigned int i = 0; i < numbers_n - 1; i++) {
    unsigned int sum = numbers[i];
    unsigned j = i;
    for (; j < numbers_n - 1 && sum < invalid_n;) {
      sum += numbers[++j];
    }

    if (sum == invalid_n && j > i) {
      range_start = i;
      range_end = j;
      break;
    }
  }

  unsigned int smallest = numbers[range_start];
  unsigned int largest = numbers[range_start];
  for (unsigned int i = range_start; i <= range_end; i++) {
    if (numbers[i] < smallest) {
      smallest = numbers[i];
    } else if (numbers[i] > largest) {
      largest = numbers[i];
    }
  }

  unsigned int answer = largest + smallest;
  printf("%d\n", answer);
  assert(answer == 13935797);

  return EXIT_SUCCESS;
}
