#include <stdlib.h>
#include<string.h> 
#include <stdio.h>
#include <err.h>

#define HASHMAP_BUCKETS 12001
#define HASHMAP_BUCKET_SIZE 42

struct hashmap_node {
    unsigned long long key;
    unsigned long value;
    // struct hashmap_node *next;
};

struct hashmap {
    struct hashmap_node **buckets;
    size_t *bucket_sizes;
};

#define hash(i) i % HASHMAP_BUCKETS

struct hashmap 
hashmap_new() {
    struct hashmap hm;
    hm.buckets = (struct hashmap_node **) malloc(sizeof (struct hashmap_node *) * HASHMAP_BUCKETS);
    if (!hm.buckets) err(EXIT_FAILURE, "could not allocate memory for hashmap buckets");

    hm.bucket_sizes = (size_t *) malloc(sizeof (size_t *) * HASHMAP_BUCKETS);

    for (size_t i=0; i < HASHMAP_BUCKETS; i++) {
        hm.bucket_sizes[i] = 0;
        hm.buckets[i] = (struct hashmap_node *) malloc(sizeof (struct hashmap_node) * HASHMAP_BUCKET_SIZE);

        for (int j=0; j < HASHMAP_BUCKET_SIZE; j++) {
            hm.buckets[i][j] = (struct hashmap_node) {
                .key = 0,
                .value = 0,
            };
        }
    }
    return hm;
}

// static struct {
//     struct hashmap_node nodes[72923]; 
//     size_t used;
//     char inited;
// } hm_node_pool = {
//     .used = 0,
// };

size_t highest_bucket_index = 0;
    
void 
hashmap_set(struct hashmap *hm, unsigned long long key, unsigned long value) {
    unsigned long h = hash(key);
    struct hashmap_node node;

    for (size_t i=0; i < hm->bucket_sizes[h]; i++) {
        node = hm->buckets[h][i];
        if (node.key == key) {
            node.value = value;
            return;
        }
    }

    // while (node != NULL) {
    //     if (node->key == key) {
    //         node->value = value;
    //         return;
    //     }
    //     node = node->next;
    // }

    if (hm->bucket_sizes[h] > highest_bucket_index) {
        highest_bucket_index = hm->bucket_sizes[h];
    }

    size_t idx = hm->bucket_sizes[h];
    printf("adding at hash %ld, index %ld\n", h, idx);
    hm->buckets[h][idx].key = key;
    hm->buckets[h][idx].value = value;
    hm->bucket_sizes[h]++;

    // otherwise, add new node
    // struct hashmap_node *new_node = &hm_node_pool.nodes[hm_node_pool.used++];
    // new_node->key =  key;
    // new_node->value = value;
    // new_node->next = hm->buckets[h];
    // hm->buckets[h] = new_node;
    return;
}

unsigned long hashmap_sum(struct hashmap *hm) {
    unsigned long long sum = 0;
    for (int h = 0; h < HASHMAP_BUCKETS; h++) {
        for (size_t i=0; i < hm->bucket_sizes[h]; i++) {
            sum += hm->buckets[h][i].value;
        }
    }
    return sum;
}