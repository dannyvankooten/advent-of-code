#include <stdlib.h>
#include <string.h> 
#include <err.h>
#include <stdio.h>

#define HASHMAP_BUCKETS 26001
#define hash(i) i % HASHMAP_BUCKETS

struct hashmap_node {
    int key;
    int value;
    struct hashmap_node *next;
};

struct hashmap {
    struct hashmap_node **buckets;
};

struct hashmap 
hashmap_new(size_t cap) {
    struct hashmap hm;
    hm.buckets = (struct hashmap_node **) malloc(sizeof (struct hashmap_node *) * cap);
    if (hm.buckets == NULL) {
        err(EXIT_FAILURE, "error allocating memory for hashmap buckets");
    }

    for (size_t i=0; i < cap; i++) {
        hm.buckets[i] = NULL;
    }
    return hm;
}

static struct {
    struct hashmap_node *nodes; 
    size_t used;
    size_t cap;
    char inited;
} hm_node_pool = {
    .used = 0,
    .cap = 0,
    .inited = 0,
};
    
void 
hashmap_set(struct hashmap *hm, int key, int value) {
    size_t h = hash(key);
    struct hashmap_node *node = hm->buckets[h];
   
    while (node != NULL) {
        if (node->key == key) {
            node->value = value;
            return;
        }
        node = node->next;
    }

    if (0 == hm_node_pool.inited) {
        hm_node_pool.nodes = malloc(1000000 * sizeof(struct hashmap_node));
        hm_node_pool.cap = 1000000;
        hm_node_pool.inited = 1;
    }

    // otherwise, add new node
    //printf("Adding node to bucket %ld\n", h);
    //node = malloc(sizeof (* node));
    node = &hm_node_pool.nodes[hm_node_pool.used++];
    node->key =  key;
    node->value = value;
    node->next = hm->buckets[h];
    hm->buckets[h] = node;
    return;
}

int 
hashmap_getset(struct hashmap *hm, int key, int new_value) {
    size_t h = hash(key);
    struct hashmap_node *node = hm->buckets[h];
    int old_value;

    while (node != NULL) {
        if (node->key == key) {
            old_value = node->value;
            node->value = new_value;
            return old_value;
        }
        node = node->next;
    }

    if (0 == hm_node_pool.inited) {
        hm_node_pool.cap = 4096 * 1024;
        hm_node_pool.nodes = malloc(hm_node_pool.cap  * sizeof(struct hashmap_node));
        if (!hm_node_pool.nodes) err(EXIT_FAILURE, "could not allocate nodes for hashmap object pool");
        hm_node_pool.inited = 1;
    }

    node = &hm_node_pool.nodes[hm_node_pool.used++];
    node->key =  key;
    node->value = new_value;
    node->next = hm->buckets[h];
    hm->buckets[h] = node;
    return -1;
}
