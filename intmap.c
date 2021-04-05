
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <err.h>
#include "intmap.h"

intmap_t *
intmap_new(size_t size) {
    intmap_t *hm = (intmap_t *) malloc(sizeof(intmap_t));
    if (!hm) {
        err(EXIT_FAILURE, "could not allocate memory for hashmap");
    }
    hm->cap = size * 3;
    hm->entries = (intmap_entry_t *) calloc(hm->cap, sizeof(intmap_entry_t));
    if (!hm->entries) {
        err(EXIT_FAILURE, "could not allocate memory for hashmap entries");
    }
    // hm->keys = (uint32_t *) calloc(hm->cap, sizeof(uint32_t));
    // hm->values = (uint32_t *) malloc(hm->cap * sizeof(uint32_t));
    return hm;
}

static inline size_t
hash_func (size_t key)
{
  key = ((key >> 16) ^ key) * 0x45d9f3b;
  key = ((key >> 16) ^ key) * 0x45d9f3b;
  key = (key >> 16) ^ key;
  return key;
}

int64_t 
intmap_get(intmap_t *hm, size_t key) {
    size_t index = hash_func(key) % hm->cap;
    u_int8_t tries = 0;

    while (hm->entries[index].key != key) {
         // exhausted linear probing attempts
        // assume item was not in hashmap yet
        if (++tries > 5) {
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
intmap_set(intmap_t *hm, size_t key, int64_t value) {
    size_t index = hash_func(key) % hm->cap;

    u_int8_t tries = 0;
    while (hm->entries[index].key != 0) {
        if (hm->entries[index].key == key) {
            int64_t old_value = hm->entries[index].value;
            hm->entries[index].value = value;
            return old_value;
        }

        // did we exchaust # of tries?        
        tries++;
        if (tries > 30) {
            err(EXIT_FAILURE, "exhausted linear probing attempts");
        }

        // try next index
        index++;

        // wrap around if we reached end of entries
        if (index >= hm->cap) {
            index = 0;
        }
    }

    // add to values
    hm->entries[index].key = key;
    hm->entries[index].value = value;
    return 0;
    // hm->keys[index] = key;
    // hm->values[index] = value;
}

void
intmap_free(intmap_t *m) {
    free(m->entries);
    free(m);
}