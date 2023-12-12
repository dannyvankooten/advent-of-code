#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

typedef struct edge {
    char a[32];
    char b[32];
    int distance;
} edge_t;

typedef struct node {
    char name[32];
    struct node *neighbors[16];
    int nneighbors;
    int distances[16];
} node_t;

node_t *getnode(node_t nodes[], int nnodes, char *name) {
    for (int i = 0; i < nnodes; i++){
        if (strncmp(nodes[i].name, name, 32) == 0) {
            return &nodes[i];
        }
    }

    return NULL;
}

int indexof(node_t nodes[], int nnodes, const char *name) {
    for (int i = 0; i < nnodes; i++){
        if (strcmp(nodes[i].name, name) == 0) {
            return i;
        }
    }

    abort();
    return -1;
}

char *parse_int(int *dst, char *src) {
    int n  = 0;
    while (*src >= '0' && *src <= '9') {
        n = (n * 10) + (*src++ - '0');
    }

    *dst = n;
    return src;
}

char *parse_name(char *dst, char *src) {
    while (*src != ' ') {
        *dst++ = *src++;
    }
    *dst = '\0';
    return src;
}

int sum(const int values[], int nvalues) {
    int s = 0;
    for (int i=0; i < nvalues; i++) {
        s += values[i];
    }
    return s;
}


int dijkstra(node_t nodes[], int nnodes, int startnode) {
    int distances[nnodes];
    int visited[nnodes];
    for (int i = 0; i < nnodes; i++) {
        distances[i] = 1 << 15;
        visited[i] = 0;
    }
    distances[startnode] = 0;

    // walk graph
    while (sum(visited, nnodes) < nnodes) {
        int idx = -1;
        for (int i = 0; i < nnodes; i++) {
            if (visited[i] == 1) {
                continue;
            }

            if (idx == -1 || distances[i] < distances[idx]) {
                idx = i;
            }
        }
        assert(idx != -1);

        node_t *cur = &nodes[idx];
//        printf("#  %s (%d) (cum distance = %d)\n", cur->name, idx, distances[idx]);
        visited[idx] = 1;
        int distance = distances[idx];

        for (int i = 0; i < cur->nneighbors; i++) {
//            printf("%d to %s (cum distance = %d)\n", cur->distances[i], cur->neighbors[i]->name, distance + cur->distances[i]);
            node_t *v = getnode(nodes, nnodes, cur->neighbors[i]->name);
            int vi = indexof(nodes, nnodes, v->name);
            int d = distance + cur->distances[i];
            if (d < distances[vi]) {
                distances[vi] = d;
//                printf("new shortest distance from %s to %s = %d\n", cur->name, v->name, d);
            }
        }
//        printf("\n");
    }

    return sum(distances, nnodes);
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    char input[32*1024] = "";
    size_t nread = fread(input, 1, 32*1024, fp);
    input[nread] = '\0';
    fclose(fp);

    clock_t start_t, end_t;
    start_t = clock();

    char *s = input;
    int pt1 = 0;

    // parse input
    edge_t *edges = malloc(64 * sizeof(edge_t));
    if (edges == NULL) {
        exit(EXIT_FAILURE);
    }
    int nedges = 0;
    while (*s != '\0') {
        edge_t e;
        s = parse_name(e.a, s);
        s += 4;
        s = parse_name(e.b, s);
        s += 3;
        s = parse_int(&e.distance, s);
        s++;

        edges[nedges++] = e;
    }

    // build graph
    node_t nodes[16];
    int nnodes = 0;
    for (int i = 0; i < nedges; i++) {
        edge_t e = edges[i];

        node_t *a = getnode(nodes, nnodes, e.a);
        if (a == NULL) {
            a = (node_t *) &nodes[nnodes++];
            a->nneighbors = 0;
            strncpy(a->name, e.a, 32);
        }

        node_t *b = getnode(nodes, nnodes, e.b);
        if (b == NULL) {
            b = (node_t *) &nodes[nnodes++];
            b->nneighbors = 0;
            strncpy(b->name, e.b, 32);
        }

        a->neighbors[a->nneighbors] = b;
        a->distances[a->nneighbors] = e.distance;
        a->nneighbors++;

        b->neighbors[b->nneighbors] = a;
        b->distances[b->nneighbors] = e.distance;
        b->nneighbors++;
    }


//    printf("%d cities\n", nnodes);
//    for (int i = 0; i < nnodes; i++) {
//        printf("%d. %s (%d neighbors)\n", i, nodes[i].name, nodes[i].nneighbors);
//    }

    assert(indexof(nodes, nnodes, "AlphaCentauri") == 0);
    assert(indexof(nodes, nnodes, "Snowdin") == 1);
    assert(indexof(nodes, nnodes, "Tambi") == 2);
    assert(indexof(nodes, nnodes, "Faerun") == 3);
    assert(indexof(nodes, nnodes, "Norrath") == 4);
    assert(indexof(nodes, nnodes, "Straylight") == 5);
    assert(indexof(nodes, nnodes, "Tristram") == 6);
    assert(indexof(nodes, nnodes, "Arbre") == 7);

    printf("--- Day 9: All in a Single Night ---\n");
    pt1 = dijkstra(nodes, nnodes, 0);
//    for (int i = 1; i < nnodes; i++) {
//        int tmp = dijkstra(nodes, nnodes, i);
//        pt1 = tmp < pt1 ? tmp : pt1;
//    }
    printf("Part 1: %d\n", pt1);
    printf("Part 2: %d\n", pt1);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);

    free(edges);
    return 0;
}
