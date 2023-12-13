#include "../adventofcode.h"
#include <assert.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  for (int i = 0; i < nnodes; i++) {
    if (strncmp(nodes[i].name, name, 32) == 0) {
      return &nodes[i];
    }
  }

  return NULL;
}

int indexof(node_t nodes[], int nnodes, const char *name) {
  for (int i = 0; i < nnodes; i++) {
    if (strcmp(nodes[i].name, name) == 0) {
      return i;
    }
  }

  abort();
  return -1;
}

char *parse_name(char *dst, char *src) {
  while (*src != ' ') {
    *dst++ = *src++;
  }
  *dst = '\0';
  return src;
}

int closest_node(int distances[], char visited[], int nnodes) {
  int min = 1 << 30;
  int min_index = -1;

  for (int v = 0; v < nnodes; v++) {
    if (visited[v] == 0 && distances[v] != -1 && distances[v] <= min) {
      min = distances[v], min_index = v;
    }
  }

  return min_index;
}

int furthest_node(int dist[], char visited[], int nnodes) {
  int max = -1;
  int max_index = -1;

  for (int v = 0; v < nnodes; v++) {
    if (visited[v] == 0 && dist[v] > max) {
      max = dist[v], max_index = v;
    }
  }
  return max_index;
}

int dijkstra(node_t *nodes, int nnodes, int src,
             int (*next)(int dist[], char visited[], int nnodes)) {
  int distances[nnodes];
  char visited[nnodes];
  for (int i = 0; i < nnodes; i++) {
    distances[i] = -1;
    visited[i] = 0;
  }
  distances[src] = 0;
  int u = src;

  for (int count = 0; count < nnodes; count++) {
    u = next(distances, visited, nnodes);
    visited[u] = 1;

    node_t *cur = &nodes[u];
    for (int i = 0; i < cur->nneighbors; i++) {
      int n = indexof(nodes, nnodes, cur->neighbors[i]->name);
      distances[n] = distances[u] + cur->distances[i];
    }
  }

  return distances[u];
}

int main() {
  clock_t tstart = timer_start();
  char input[1024 * 64] = "";
  read_input_file(input, 1024 * 64, "input.txt");

  // parse input
  edge_t *edges = malloc_or_die(64 * sizeof(edge_t));
  int nedges = 0;
  char *s = input;
  while (*s != '\0') {
    edge_t e;
    s = parse_name(e.a, s);
    s = skip(" to ", s);
    s = parse_name(e.b, s);
    s = skip(" = ", s);
    s = parse_int(&e.distance, s);
    s = skip_optional('\n', s);

    edges[nedges++] = e;
  }

  // build graph
  node_t nodes[16];
  int nnodes = 0;
  for (int i = 0; i < nedges; i++) {
    edge_t e = edges[i];

    node_t *a = getnode(nodes, nnodes, e.a);
    if (a == NULL) {
      a = &nodes[nnodes++];
      a->nneighbors = 0;
      strncpy(a->name, e.a, 32);
    }

    node_t *b = getnode(nodes, nnodes, e.b);
    if (b == NULL) {
      b = &nodes[nnodes++];
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

  int a1 = 1 << 30;
  int a2 = 0;
  for (int i = 0; i < nnodes; i++) {
    int tmp = dijkstra(nodes, nnodes, i, closest_node);
    a1 = tmp < a1 ? tmp : a1;

    tmp = dijkstra(nodes, nnodes, i, furthest_node);
    a2 = tmp > a2 ? tmp : a2;
  }

  printf("--- Day 9: All in a Single Night ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", timer_stop(tstart));

  free(edges);
  return 0;
}

// wrong answers:
// 244 (too high)