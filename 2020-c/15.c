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
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

static unsigned int
parse_input(unsigned int* numbers, const char* s) {
  unsigned int n = 0;

  while (*s != '\0') {
    unsigned int num = 0;
    while (*s >= '0' && *s <= '9') {
      num = (num * 10) + (unsigned int) (*s++ - '0');
    }

    numbers[n++] = num;
    if (*s == ',') {
      s++;
    }
  }

  return n;
}


int day15(void) {
  unsigned int numbers[6];
  unsigned int nnumbers = parse_input(numbers, "12,1,16,3,11,0");

  char seen_malloced = 0;
  unsigned int* seen = hugemem(N * sizeof(unsigned int));
  if (seen == NULL) {
     seen = malloc(N * sizeof(unsigned int));
     seen_malloced = 1;
  }
  assert(seen != NULL);

  unsigned int i = 0;
  unsigned int prev = 0;
  unsigned char* bitarray = calloc(BITNSLOTS(N), sizeof(unsigned char));
  if (!bitarray) {
    perror("calloc error");
    return 1;
  }

  for (; i < nnumbers; i++) {
    prev = numbers[i];
    seen[prev] = i + 1;
    BITSET(bitarray, prev);
  }

  unsigned int last_seen_at;
  for (; i < N; i++) {
    if (likely(prev < (i >> 8))) {
      last_seen_at = seen[prev];
      seen[prev] = i;
      if (last_seen_at) {
        prev = i - last_seen_at;
      } else {
        prev = 0;
      }
    } else if (unlikely(BITTEST(bitarray, prev))) {
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
  free(bitarray);
  if (seen_malloced) {
    free(seen);
  } else {
    hugemem_free((void *) seen, N * sizeof(int));
  }

  return 0;
}
