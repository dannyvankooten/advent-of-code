#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define N 30000000
#define BOUNDARY 1000000

typedef struct {
    uint32_t key;
    uint32_t value;
} hashmap_entry_t;

typedef struct {
    // uint32_t *keys;
    // uint32_t *values;
    hashmap_entry_t *entries;
    uint32_t cap;
} hashmap_t;


hashmap_t *
hm_new(uint32_t size) {
    hashmap_t *hm = (hashmap_t *) malloc(sizeof(hashmap_t));
    hm->cap = size * 1;
    hm->entries = (hashmap_entry_t *) calloc(hm->cap, sizeof(hashmap_entry_t));
    // hm->keys = (uint32_t *) calloc(hm->cap, sizeof(uint32_t));
    // hm->values = (uint32_t *) malloc(hm->cap * sizeof(uint32_t));
    return hm;
}

uint32_t 
hm_get(hashmap_t *hm, uint32_t key) {
    uint32_t index = key % hm->cap;
    u_int8_t tries = 0;
    while (hm->entries[index].key != key) {
         // exhausted linear probing attempts
        // assume item was not in hashmap yet
        tries++;
        if (tries > 1) {
            return 0;
        }

        index++;
        // wrap around
        if (index >= hm->cap) {
            index = 0;
        }
    }

    return hm->entries[index].value;
}

void 
hm_set(hashmap_t *hm, uint32_t key, uint32_t value) {
    // uint32_t hash = hash_func(key);
    uint32_t index = key % hm->cap;
    u_int8_t tries = 0;
    while (hm->entries[index].key != 0) {
        if (hm->entries[index].key == key) {
            hm->entries[index].value = value;
            return;
        }

        // did we exchaust # of tries?        
        tries++;
        if (tries > 3) {
            exit(1);
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
    // hm->keys[index] = key;
    // hm->values[index] = value;
}

static 
uint32_t   
parse_input(uint32_t *numbers, char *s) {
    uint32_t num;
    uint32_t n = 0;

    while (*s != '\0') {
        num = 0;
        while (*s >= '0' && *s <= '9') {
            num = (num * 10) + (*s - '0');
            s++;
        }

        numbers[n++] = num;
        if (*s == ',') s++;
    }
    
    return n;
}

int day15() {
    uint32_t numbers[6];
    uint32_t nnumbers = parse_input(numbers, "12,1,16,3,11,0");        
    // uint32_t *seen = calloc(BOUNDARY, sizeof(uint32_t));
    uint32_t seen[BOUNDARY] = {0};
    hashmap_t * ht = hm_new(N-BOUNDARY);
    uint32_t i;
    for (i=0; i < nnumbers; i++) {
        seen[numbers[i]] = i+1;
    }

    uint32_t j;
    uint32_t cur = numbers[nnumbers-1];

    for (; i < N; i++) {
        if (cur < BOUNDARY) {
            j = seen[cur];
            seen[cur] = i;
            if (j > 0) {
                cur = i - j;
            } else {
                cur = 0;
            }
        } else {
            j = hm_get(ht, cur);
            // printf("Got from hashmap: %d seen at %d\n", cur, j);
            // printf("Setting in hashmap: %d seen at %d\n", cur, i);

            hm_set(ht, cur, i);
            if (j > 0) {
                cur = i - j;
            } else {
                cur = 0;
            }
        }
    }

    // hash_free(ht);

    free(ht->entries);
    free(ht);
    printf("%d\n", cur);  
    // free(seen);
    return 0;
}
