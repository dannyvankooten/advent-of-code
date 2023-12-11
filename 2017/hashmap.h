#include <stdlib.h>
#include <stdint.h>

// hash length bytes of data
unsigned int hash(const unsigned char *data, unsigned int length) {
    const unsigned int fnv_prime = 0x811C9DC5;
    unsigned int hash = 0;

    for (unsigned int i = 0; i < length; data++, i++)
    {
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
    h.entries =  malloc(cap * sizeof(hashmap_entry_t));
    for (unsigned int i = 0; i < cap; i++) {
        h.entries[i].key = 0;
    }
    return h;
}

void hashmap_set(hashmap_t *h, unsigned char *data, unsigned int datasize, int value) {
    unsigned int key = hash(data, datasize);
    unsigned int k = key & (h->cap - 1);
    unsigned int probes = 0;

    // store at next free spot
    while (h->entries[k].key != 0 && probes++ < HM_MAX_PROBES) {
        k = k == (h->cap - 1) ? 0 : k+1;
    }

    if (probes == HM_MAX_PROBES) {
        abort();
    }

    h->entries[k].key = key;
    h->entries[k].value = value;
}

int *hashmap_get(hashmap_t *h, unsigned char *data, unsigned int datasize) {
    unsigned int key = hash(data, datasize);
    unsigned int k = key & (h->cap - 1);
    unsigned int probes = 0;
    while (h->entries[k].key != key && probes < HM_MAX_PROBES) {
        k = k == (h->cap - 1) ? 0 : k+1;
        probes++;
    }

    if (probes == HM_MAX_PROBES) {
        return NULL;
    }

    return &h->entries[k].value;
}

void hashmap_free(hashmap_t *h) {
    free(h->entries);
}