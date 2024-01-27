#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "hugemem.h"

// const char *test_input = "389125467";
const char* input = "327465189";

static void print_raw(const int* restrict cups);

static int
parse_input(int* restrict cups, const char* restrict s) {
  int i = 1;
  int first = (*s++ - '0');
  int prev = first;

  for (; *s != '\0'; s++, i++) {
    cups[prev] = (*s - '0');
    prev = cups[prev];
  }

  // fill in remainder with incrementing values
  // for (i=i+1; i < 1000000; i++) {
  for (; i < 1000000; i++) {
    cups[prev] = i + 1;
    prev = cups[prev];
  }

  // last element should point  to first
  cups[prev] = first;

  return first;
}

static void
print_raw(const int* restrict cups) {
  printf("| ");
  for (int i = 1; i <= 9; i++) {
    printf("%2d | ", i);
  }
  printf("\n| ");
  for (int i = 1; i <= 9; i++) {
    printf("%2d | ", cups[i]);
  }
  printf("\n\n");
}

static void
print_cups(const int* restrict cups, const int first, const size_t n, const char* restrict before) {
  printf("%s", before);
  int c = first;
  for (size_t i = 0; i < n; i++, c = cups[c]) {
    printf("%s%d", i > 0 ? ", " : "", c);
  }
  printf("\n");
}

int day23() {
  int *cups = (int *) hugemem(1000000 * sizeof(int));
  if (!cups) {
    perror("hugemem error");
    return 1;
  }
  int current_cup = parse_input(cups, input);
  int a, b, c;
  int destination;
  assert(current_cup == 3);
  int next_cur;

  for (int m = 0; m < 10000000; m++) {
    a = cups[current_cup];
    b = cups[a];
    c = cups[b];
    cups[current_cup] = next_cur = cups[c];

    destination = current_cup;
    do {
      destination--;
      if (destination < 1) {
        destination = 1000000;
      }
    } while (destination == a || destination == b || destination == c);

    cups[c] = cups[destination];
    cups[destination] = a;
    current_cup = next_cur;
  }

  int c1 = cups[1];
  int64_t r = (int64_t)c1 * (int64_t)cups[c1];
  printf("%ld\n", r);
  assert(r == 474600314018);
  hugemem_free(cups, 1000000 * sizeof(int));
  return 0;
}
