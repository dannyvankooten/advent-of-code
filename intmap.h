
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t key;
    int64_t value;
} intmap_entry_t;

typedef struct {
    // uint32_t *keys;
    // uint32_t *values;
    intmap_entry_t *entries;
    size_t cap;
} intmap_t;

intmap_t *
intmap_new(size_t size);

int64_t 
intmap_get(intmap_t *hm, const size_t key);

int64_t  
intmap_set(intmap_t *hm, const size_t key, int64_t value);

void 
intmap_free(intmap_t *m);