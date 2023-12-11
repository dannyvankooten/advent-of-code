#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>     /* for CHAR_BIT */
#include "hugemem.h"

#define N 30000000
#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

static int
parse_input(int* numbers, const char* s) {
  int n = 0;

  while (*s != '\0') {
    int num = 0;
    while (*s >= '0' && *s <= '9') {
      num = (num * 10) + (*s++ - '0');
    }

    numbers[n++] = num;
    if (*s == ',') {
      s++;
    }
  }

  return n;
}

int day15() {
  int numbers[6];
  int nnumbers = parse_input(numbers, "12,1,16,3,11,0");

  int* seen = hugemem(N * sizeof(int));
  if (seen == NULL) {
    perror("hugemem error");
    return 1;
  }
  assert(seen != NULL);

  int i = 0;
  int prev = 0;
  char* bitarray = calloc(BITNSLOTS(N), sizeof(char));

  for (; i < nnumbers; i++) {
    prev = numbers[i];
    seen[prev] = i + 1;
    BITSET(bitarray, prev);
  }

  int last_seen_at;
  for (; i < N; i++) {
    if (prev < (i >> 6)) {
      last_seen_at = seen[prev];
      seen[prev] = i;
      if (last_seen_at) {
        prev = i - last_seen_at;
      } else {
        prev = 0;
      }
    } else if(BITTEST(bitarray, prev)) {
      last_seen_at = seen[prev];
      seen[prev] = i;
      prev = i - last_seen_at;
    } else {
      BITSET(bitarray, prev);
      seen[prev] = i;
      prev = 0;
    }

  }

  printf("%d\n", prev);
  assert(prev == 37385);

  hugemem_free((void *) seen, N * sizeof(int));

  return 0;
}
