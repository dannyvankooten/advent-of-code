#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "inputs/24.h"

#define GRIDSIZE 140
#define GRIDSIZE_SQ GRIDSIZE * GRIDSIZE
#define isodd(v) (v & 1)
#define iseven(v) (v & 1) == 0

enum color {
  WHITE = 0,
  BLACK = 1,
};

typedef enum direction { NE, E, SE, SW,  W, NW } direction_t;

typedef struct {
  int32_t black_tile_count;
  char* tiles;
} grid_t;

typedef struct {
  int32_t min;
  int32_t max;
} bound_t;

static const char directions[2][6][2] = {
  {
    // even rows
    { 0, -1 },  // NE
    { 1, 0 },   // E
    { 0, 1 },   // SE
    { -1, 1 },  // SW
    { -1, 0 },  // W
    { -1, -1 }, // NW
  },
  {
    // odd rows
    { 1, -1},   // NE
    { 1, 0},   // E
    { 1, 1},  // SE
    { 0, 1},   // SW
    { -1, 0},   // W
    { 0, -1},   // NW
  }
};

void err(int status, char *message) {
  fputs(message, stderr);
  exit(status);
}

static void
print_direction(const direction_t d) {
  static const char* names[] = {"NE", "NW", "E", "W", "SE", "SW"};
  printf("%s", names[d]);
}

static int32_t
parse_line(direction_t* restrict directions, const unsigned char* restrict s) {
  int32_t ndirections = 0;
  while (*s != '\n' && *s != '\0') {
    switch (*s++) {
      case 'e':
        directions[ndirections++] = E;
        break;

      case 'w':
        directions[ndirections++] = W;
        break;

      case 'n':
        switch (*s++) {
          case 'e':
            directions[ndirections++] = NE;
            break;

          case 'w':
            directions[ndirections++] = NW;
            break;
        }
        break;

      case 's':
        switch (*s++) {
          case 'e':
            directions[ndirections++] = SE;
            break;

          case 'w':
            directions[ndirections++] = SW;
            break;
        }
        break;
    }
  }

  return ndirections;
}

static void
apply_to_neighbors(int8_t* restrict neighbors, const int32_t x, const int32_t y, const int8_t d) {
  int8_t parity = y & 1;
  for (int8_t i=0; i < 6; i++) {
    int8_t dx = directions[parity][i][0];
    int8_t dy = directions[parity][i][1];
    neighbors[(y + dy) * GRIDSIZE + (x + dx)] += d;
  }
}

static void
apply_rules(grid_t* restrict grid, int8_t neighbors[GRIDSIZE_SQ * 2], bound_t* restrict x_bound, bound_t* restrict y_bound) {
  int8_t* new_neighbors = &neighbors[GRIDSIZE_SQ];

  // then, change grid values based on earlier count (inclusive)
  for (int32_t y = y_bound->min-1; y <= y_bound->max + 1; y++) {
    for (int32_t x = x_bound->min-1; x <= x_bound->max + 1; x++) {
      int8_t black_neighbor_count = neighbors[y * GRIDSIZE + x];
      if (black_neighbor_count == 1) {
        continue;
      }

      switch (grid->tiles[y * GRIDSIZE + x]) {
        case WHITE:
          if (black_neighbor_count == 2) {
            grid->tiles[y * GRIDSIZE + x] = BLACK;
            grid->black_tile_count++;
            apply_to_neighbors(new_neighbors, x, y, +1);

            if (y < y_bound->min) {
              y_bound->min = y;
            } else if (y > y_bound->max) {
              y_bound->max = y;
            }

            if (x < x_bound->min) {
              x_bound->min = x;
            } else if (x > x_bound->max) {
              x_bound->max = x;
            }
          }
          break;

        case BLACK:
          if (black_neighbor_count == 0 || black_neighbor_count > 2) {
            grid->tiles[y * GRIDSIZE + x] = WHITE;
            grid->black_tile_count--;
            apply_to_neighbors(new_neighbors, x, y, -1);
          }
          break;
      }
    }
  }

  // copy over new neighbors to neighbors
  memcpy(neighbors, new_neighbors, sizeof(int8_t) * GRIDSIZE_SQ);
}

int day24() {
  grid_t grid = {
    .tiles = (char*)calloc(GRIDSIZE_SQ, sizeof(char)),
    .black_tile_count = 0,
  };
  if (!grid.tiles) {
    err(EXIT_FAILURE, "error allocating memory for grid tiles");
  }

  const char unsigned *s = input;
  direction_t* dir_instructions = (direction_t*)malloc(sizeof(direction_t) * 50);
  if (!dir_instructions) {
    err(EXIT_FAILURE, "error allocating memory for directions");
  }


  bound_t x_bound = {INT32_MAX, 0 };
  bound_t y_bound = {INT32_MAX, 0 };

  while (*s != '\0') {
    int32_t n = parse_line(dir_instructions, s);
    while (*s != '\n' && *s != '\0') s++;
    if (*s == '\n') s++;

    // go back to start tile (middle of grid)
    int32_t x = GRIDSIZE / 2;
    int32_t y = x;

    // follow instructions
    for (int8_t i = 0; i < n; i++) {
      int8_t parity = y & 1;
      int8_t dx = directions[parity][dir_instructions[i]][0];
      int8_t dy = directions[parity][dir_instructions[i]][1];
      x += dx;
      y += dy;
    }

    // flip tile
    char* tile_color = &grid.tiles[(y * GRIDSIZE) + x];
     switch (*tile_color) {
      case WHITE:
        grid.black_tile_count++;
        *tile_color = BLACK;
        break;

      case BLACK:
        grid.black_tile_count--;
        *tile_color = WHITE;
        break;
    }

    // update (x, y) bounds
    if (y < y_bound.min) {
      y_bound.min = y;
    } else if (y > y_bound.max) {
      y_bound.max = y;
    }

    if (x < x_bound.min) {
      x_bound.min = x;
    } else if (x > x_bound.max) {
      x_bound.max = x;
    }
  }

  // part 1
  printf("%d\n", grid.black_tile_count);
  assert(grid.black_tile_count == 244);

  // setup initial array of neighbor counts
  int8_t neighbors[GRIDSIZE_SQ * 2] = {0};
   for (int32_t y = y_bound.min; y <= y_bound.max; y++) {
    for (int32_t x = x_bound.min; x <= x_bound.max; x++) {
      if (grid.tiles[y * GRIDSIZE + x] == BLACK) {
        apply_to_neighbors(neighbors, x, y, +1);
      }
    }
  }
  memcpy(&neighbors[GRIDSIZE_SQ], neighbors, sizeof(int8_t) * GRIDSIZE_SQ);

  for (int8_t i = 0; i < 100; i++) {
    apply_rules(&grid, neighbors, &x_bound, &y_bound);
  }
  printf("%d\n", grid.black_tile_count);
  assert(grid.black_tile_count == 3665);

  free(dir_instructions);
  free(grid.tiles);
  return 0;
}
