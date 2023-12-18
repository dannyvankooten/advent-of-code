#include <stdint.h>
#include <stdlib.h>

/*
Prime capacity: use modulo
Power of 2 capacity: bit-and
*/

// hash length bytes of data
static inline unsigned int hash(const unsigned char *data, size_t length) {
  const unsigned int fnv_prime = 0x811C9DC5;
  unsigned int hash = 0;

  for (unsigned int i = 0; i < length; data++, i++) {
    hash *= fnv_prime;
    hash ^= (*data);
  }

  return hash;
}

#define HM_MAX_PROBES 16

typedef struct {
  // key holds the actual key supplied by the user
  // the entry may be stored at a different key though
  unsigned int key;
  int value;
} hashmap_entry_t;

typedef struct {
  hashmap_entry_t *entries;
  unsigned int cap;
} hashmap_t;

hashmap_t hashmap_new(unsigned int cap) {
  hashmap_t h;
  h.cap = cap;
  h.entries = (hashmap_entry_t *)calloc(cap, sizeof(hashmap_entry_t));
  if (h.entries == NULL) {
    exit(EXIT_FAILURE);
  }
  for (unsigned int i = 0; i < cap; i++) {
    h.entries[i].key = 0;
  }
  return h;
}

// hashmap_set inserts a key-value pair into the map.
int *hashmap_set(hashmap_t *h, unsigned char *data, size_t datasize,
                 int value) {
  unsigned int key = hash(data, datasize);
  unsigned int k = key % h->cap;
  unsigned int probes = 0;

  // store at next free spot
  while (h->entries[k].key != 0 && probes++ < HM_MAX_PROBES) {
    k = k == (h->cap - 1) ? 0 : k + 1;
  }

  if (probes == HM_MAX_PROBES) {
    abort();
  }

  h->entries[k].key = key;
  h->entries[k].value = value;
  return &h->entries[k].value;
}

// Gets the given key’s corresponding entry in the map for in-place
// manipulation.
int *hashmap_entry(hashmap_t *h, unsigned char *data, size_t datasize) {
  unsigned int key = hash(data, datasize);
  unsigned int k = key % h->cap;
  unsigned int probes = 0;
  while (h->entries[k].key != key && h->entries[k].key > 0 &&
         probes < HM_MAX_PROBES) {
    k = k == (h->cap - 1) ? 0 : k + 1;
    probes++;
  }

  if (probes == HM_MAX_PROBES) {
    return NULL;
  }

  if (h->entries[k].key == 0) {
    h->entries[k].key = key;
  }

  return &h->entries[k].value;
}

// hashmap_get returns a reference to the value corresponding to the key.
int *hashmap_get(hashmap_t *h, unsigned char *data, size_t datasize) {
  unsigned int key = hash(data, datasize);
  unsigned int k = key % h->cap;
  unsigned int probes = 0;
  while (h->entries[k].key != key && h->entries[k].key > 0 &&
         probes < HM_MAX_PROBES) {
    k = k == (h->cap - 1) ? 0 : k + 1;
    probes++;
  }

  if (probes == HM_MAX_PROBES) {
    return NULL;
  }

  return &h->entries[k].value;
}

void hashmap_free(hashmap_t *h) { free(h->entries); }