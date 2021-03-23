#include <stdlib.h>
#include<string.h> 

#define HASHMAP_BUCKETS 26001
#define hash(i) i % HASHMAP_BUCKETS

struct hashmap_node {
    unsigned long key;
    unsigned long value;
    struct hashmap_node *next;
};

struct hashmap {
    struct hashmap_node *buckets[HASHMAP_BUCKETS];
};

struct hashmap 
hashmap_new() {
    struct hashmap hm;
    for (int i=0; i < HASHMAP_BUCKETS; i++) {
        hm.buckets[i] = NULL;
    }
    return hm;
}

static struct {
    struct hashmap_node nodes[73000]; 
    size_t used;
    char inited;
} hm_node_pool = {
    .used = 0,
};
    
void 
hashmap_set(struct hashmap *hm, unsigned long key, int value) {
    unsigned long h = hash(key);
    struct hashmap_node *node = hm->buckets[h];

    while (node != NULL) {
        if (node->key == key) {
            node->value = value;
            return;
        }
        node = node->next;
    }

    // otherwise, add new node
    struct hashmap_node *new_node = &hm_node_pool.nodes[hm_node_pool.used++];
    new_node->key =  key;
    new_node->value = value;
    new_node->next = hm->buckets[h];
    hm->buckets[h] = new_node;
    return;
}

unsigned long hashmap_sum(struct hashmap *hm) {
    unsigned long sum = 0;
    struct hashmap_node *node;
    for (int i = 0; i < HASHMAP_BUCKETS; i++) {
        node = hm->buckets[i];
        while (node != NULL) {
            sum += node->value;
            node = node->next;
        }
    }
    return sum;
}