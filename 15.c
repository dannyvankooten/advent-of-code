#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define N 30000000
#define BOUNDARY 1000000
#define HM_PROBING_ATTEMPTS 3

typedef struct {
  int32_t key;
  int32_t value;
} hashmap_entry_t;

typedef struct {
  // int32_t *keys;
  // int32_t *values;
  hashmap_entry_t* entries;
  int32_t cap;
} hashmap_t;

hashmap_t* hm_new(int32_t size) {
  hashmap_t* hm = (hashmap_t*)malloc(sizeof(hashmap_t));
  if (!hm) {
    err(EXIT_FAILURE, "could not allocate memory for hashmap");
  }
  hm->cap = size;
  hm->entries = (hashmap_entry_t*)calloc(hm->cap, sizeof(hashmap_entry_t));
  if (!hm->entries) {
    err(EXIT_FAILURE, "could not allocate memory for hashmap entries");
  }
  // hm->keys = (int32_t *) calloc(hm->cap, sizeof(int32_t));
  // hm->values = (int32_t *) malloc(hm->cap * sizeof(int32_t));
  return hm;
}

int32_t hm_get(hashmap_t* restrict hm, int32_t key, int32_t new_value) {
  int32_t index = key % hm->cap;
  int_fast8_t tries = 0;
  
  while (hm->entries[index].key != key) {
    // exhausted linear probing attempts
    // assume item was not in hashmap yet
    if (tries++ > HM_PROBING_ATTEMPTS && hm->entries[index].key == 0) {
      hm->entries[index].key = key;
      break;
    }
   
    // try next index, wrap around if at end 
    if (index++ >= hm->cap) {
      index = 0;
    }
  }

  int32_t old_value = hm->entries[index].value;
  hm->entries[index].value = new_value;
  return old_value;
}

static int32_t parse_input(int32_t* numbers, char* s) {
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
  int32_t* seen = calloc(BOUNDARY, sizeof(int32_t));
  hashmap_t* ht = hm_new(N - BOUNDARY);

  int32_t i = 0;
  int32_t prev = 0;
  for (; i < nnumbers; i++) {
    seen[numbers[i]] = i + 1;
    prev = numbers[i];
  }

  int32_t last_seen_at;
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
  free(ht->entries);
  free(ht);
  return 0;
}
