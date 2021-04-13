#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

#define N 30000000
#define BOUNDARY 524288 // 2 ^ 20
#define HM_CAP 16777216 // 2 ^ 24
#define HM_PROBING_ATTEMPTS 2

typedef struct hashmap hashmap_t;
struct hashmap {
  uint32_t cap;
  uint32_t *keys;
  uint32_t *values;
};

static hashmap_t* 
hm_new() {
  hashmap_t* hm = (hashmap_t*)malloc(sizeof(hashmap_t));
  if (!hm) {
    err(EXIT_FAILURE, "could not allocate memory for hashmap");
  }
  hm->cap = HM_CAP;
  uint32_t *space = (uint32_t*) calloc(hm->cap * 2, sizeof(uint32_t));
  if (!space) {
    err(EXIT_FAILURE, "error allocating memory for hashmap keys and values");
  }
  hm->keys = space;
  hm->values = space + hm->cap;
  return hm;
}

static uint32_t 
hm_get(hashmap_t* restrict hm, const uint32_t key, const uint32_t new_value) {
  uint32_t index = key & (HM_CAP - 1); 
  uint_fast8_t tries = 0;

  while (hm->keys[index] != key) {
    // exhausted linear probing attempts
    // assume item was not in hashmap yet
    if (++tries >= HM_PROBING_ATTEMPTS && hm->keys[index] == 0) {
      hm->keys[index] = key;
      break;
    }
   
    // try next index, wrap around if at end 
    if (++index >= hm->cap) {
      index = 0;
    }
  }

  uint32_t old_value = hm->values[index];
  hm->values[index] = new_value;
  return old_value;
}

static uint32_t 
parse_input(uint32_t* restrict numbers, const char* s) {
  uint32_t n = 0;

  while (*s != '\0') {
    uint32_t num = 0;
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
  uint32_t numbers[6];
  uint32_t nnumbers = parse_input(numbers, "12,1,16,3,11,0");
  uint32_t* restrict seen = calloc(BOUNDARY, sizeof(uint32_t));
  hashmap_t* restrict ht = hm_new();
  uint32_t i = 0;
  uint32_t prev = 0;
  for (; i < nnumbers; i++) {
    seen[numbers[i]] = i + 1;
    prev = numbers[i];
  }

  uint32_t last_seen_at;
  for (; i < N; i++) {
    if (prev < BOUNDARY) {
      last_seen_at = seen[prev];
      seen[prev] = i;
    } else {
      last_seen_at = hm_get(ht, prev, i);
    }

    if (last_seen_at > 0) {
      prev = i - last_seen_at;
    } else {
      prev = 0;
    }
  }

  printf("%d\n", prev);
  assert(prev == 37385);

  free(seen);
  free(ht->keys);
  free(ht);
  return 0;
}
