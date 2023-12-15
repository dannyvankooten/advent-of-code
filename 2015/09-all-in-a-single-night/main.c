#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int next_pt1(int distances[16], int visited[16], int n) {
  int min = -1;
  int min_index = -1;

  for (int i = 0; i < n; i++) {
    if (visited[i] == 1 || distances[i] == -1) {
      continue;
    }

    if (min == -1 || distances[i] < min) {
      min = distances[i];
      min_index = i;
    }
  }

  return min_index;
}

int next_pt2(int distances[16], int visited[16], int n) {
  int max = -1;
  int max_index = -1;

  for (int i = 0; i < n; i++) {
    if (visited[i] == 1 || distances[i] == -1) {
      continue;
    }

    if (max == -1 || distances[i] > max) {
      max = distances[i];
      max_index = i;
    }
  }

  return max_index;
}

int dijkstra(int graph[16][16], int n, int start,
             int next(int[16], int[16], int)) {

  int distances[16];
  int visited[16];
  for (int i = 0; i < n; i++) {
    distances[i] = -1;
    visited[i] = 0;
  }
  distances[start] = 0;

  int u = start;
  for (int i = 0; i < n; i++) {
    u = next(distances, visited, n);
    visited[u] = 1;

    for (int v = 0; v < n; v++) {
      if (visited[v] == 1) {
        continue;
      }

      if (graph[u][v] == 0) {
        continue;
      }

      distances[v] = distances[u] + graph[u][v];
    }
  }

  // return (total) distance to last node
  return distances[u];
}

int getcity(char cities[16][16], int ncities, char *c) {
  for (int i = 0; i < ncities; i++) {
    if (strcmp(cities[i], c) == 0) {
      return i;
    }
  }

  // not found
  return -1;
}

int main() {
  clock_t tstart = clock_time();
  char input[1024 * 64] = "";
  read_input_file(input, 1024 * 64, "input.txt");

  int graph[16][16];
  memset(graph, 0, 16 * 16 * sizeof(int));
  char cities[16][16];
  int ncities = 0;
  char city_from[16];
  char city_to[16];
  int distance;
  const char *s = input;
  while (*s != '\0') {
    s = parse_ident(city_from, s);
    s = skip(" to ", s);
    s = parse_ident(city_to, s);
    s = skip(" = ", s);
    s = parse_int(&distance, s);
    s = skip_optional('\n', s);

    int va = getcity(cities, ncities, city_from);
    if (va == -1) {
      va = ncities;
      strcpy(cities[ncities++], city_from);
    }

    int vb = getcity(cities, ncities, city_to);
    if (vb == -1) {
      vb = ncities;
      strcpy(cities[ncities++], city_to);
    }

    graph[va][vb] = distance;
    graph[vb][va] = distance;
  }

  int a1 = dijkstra(graph, ncities, 0, next_pt1);
  int a2 = dijkstra(graph, ncities, 0, next_pt2);
  int tmp;
  for (int i = 1; i < ncities; i++) {
    tmp = dijkstra(graph, ncities, i, next_pt1);
    a1 = tmp < a1 ? tmp : a1;

    tmp = dijkstra(graph, ncities, i, next_pt2);
    a2 = tmp > a2 ? tmp : a2;
  }

  printf("--- Day 9: All in a Single Night ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", clock_time_since(tstart));
  return EXIT_SUCCESS;
}