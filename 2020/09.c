#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "inputs/09.h"

int day9() {
  const unsigned char *s = input;
  int numbers[1000];
  int numbers_n = 0;
  int invalid_n = 104054607;
  while (*s != '\0') {
    int n = 0;
    while (*s >= '0' && *s <= '9') {
      n = n * 10 + (*s - '0');
      s++;
    }
    numbers[numbers_n++] = n;

    if (*s == '\n') s++;
  }

  // loop through numbers to find contiguous set
  // that sums to invalid_n (127)
  int range_start = 0;
  int range_end = 0;
  for (int i = 0; i < numbers_n - 1; i++) {
    int sum = numbers[i];
    int j = i;
    for (; j < numbers_n - 1 && sum < invalid_n;) {
      sum += numbers[++j];
    }

    if (sum == invalid_n && j > i) {
      range_start = i;
      range_end = j;
      break;
    }
  }

  int smallest = numbers[range_start];
  int largest = numbers[range_start];
  for (int i = range_start; i <= range_end; i++) {
    if (numbers[i] < smallest) {
      smallest = numbers[i];
    } else if (numbers[i] > largest) {
      largest = numbers[i];
    }
  }

  int answer = largest + smallest;
  printf("%d\n", answer);
  assert(answer == 13935797);

  return EXIT_SUCCESS;
}
