
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "intmap.h"

#define INTMAP_MAX_PROBING_TRIES 10

static void err(int status, const char * const message) {
  fputs(message, stderr);
  exit(status);
}

intmap_t *
intmap_new(const size_t size) {
    intmap_t *hm = (intmap_t *) malloc(sizeof(intmap_t));
    if (!hm) {
        err(EXIT_FAILURE, "could not allocate memory for hashmap");
    }

    // make sure capacity is power of 2
    hm->cap = 16384;
    while (hm->cap < size) {
        hm->cap *= 2;
    }
    hm->entries = (intmap_entry_t *) calloc(hm->cap, sizeof(intmap_entry_t));
    if (!hm->entries) {
        err(EXIT_FAILURE, "could not allocate memory for hashmap entries");
    }

    return hm;
}

static inline size_t
hash_func (size_t key) {
  key = ((key >> 16) ^ key) * 0x45d9f3b;
  key = (key >> 16) ^ key;
  return key;
}

int64_t
intmap_get(intmap_t *hm, const size_t key) {
    size_t index = hash_func(key) & (hm->cap - 1);
    uint8_t tries = 0;

    while (hm->entries[index].key != key) {
        // exhausted linear probing attempts
        if (++tries > INTMAP_MAX_PROBING_TRIES) {
            // assume item was not in hashmap yet
            return 0;
        }

        // wrap around
        if (++index >= hm->cap) {
            index = 0;
        }
    }

    return hm->entries[index].value;
}

int64_t
intmap_set(intmap_t *hm, const size_t key, const int64_t value) {
    size_t index = hash_func(key) & (hm->cap - 1);
    uint8_t tries = 0;
    while (hm->entries[index].key != 0) {
        if (hm->entries[index].key == key) {
            int64_t old_value = hm->entries[index].value;
            hm->entries[index].value = value;
            return old_value;
        }

        // did we exchaust # of tries?
        if (++tries > INTMAP_MAX_PROBING_TRIES) {
            err(EXIT_FAILURE, "exhausted linear probing attempts");
        }

        // wrap around if we reached end of entries
        if (++index >= hm->cap) {
            index = 0;
        }
    }

    // add to values
    hm->entries[index].key = key;
    hm->entries[index].value = value;
    return 0;
}

void
intmap_free(intmap_t *m) {
    free(m->entries);
    free(m);
}
