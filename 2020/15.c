#define _GNU_SOURCE
#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>     /* for CHAR_BIT */
#include <sys/mman.h>
#include <linux/mman.h>
#include "hugemem.h"

#define N 30000000
#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

static int32_t
parse_input(int32_t* restrict numbers, const char* s) {
  int32_t n = 0;

  while (*s != '\0') {
    int32_t num = 0;
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
  int32_t numbers[6];
  int32_t nnumbers = parse_input(numbers, "12,1,16,3,11,0");

  uint32_t* seen = (uint32_t*) hugemem(N * sizeof(uint32_t));
  if (seen == NULL) {
    perror("hugemem error");
    return 1;
  }
  assert(seen != NULL);
  // memset(seen, 0, (N * sizeof(uint32_t)));

  int32_t i = 0;
  int32_t prev = 0;
  // char bitarray[BITNSLOTS(N)] = {0};
  char* bitarray = calloc(BITNSLOTS(N), sizeof(char));

  for (; i < nnumbers; i++) {
    prev = numbers[i];
    seen[prev] = i + 1;
    BITSET(bitarray, prev);
  }

  uint32_t last_seen_at;
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

  hugemem_free((void *) seen, N * sizeof(uint32_t));

  return 0;
}
