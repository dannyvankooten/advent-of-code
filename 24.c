#include <assert.h>
#include <err.h>
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

typedef enum {
  WHITE = 0,
  BLACK = 1,
} color_t;

typedef enum { NE, E, SE, SW,  W, NW } direction_t;

typedef struct {
  int32_t black_tile_count;
  color_t* tiles;
  color_t* new_tiles;
} grid_t;

static const int8_t directions[2][6][2] = {
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

void flip_tile(color_t* restrict color, int32_t* restrict black_tile_count) {
  switch (*color) {
    case WHITE:
      (*black_tile_count)++;
      *color = BLACK;
      break;

    case BLACK:
      (*black_tile_count)--;
      *color = WHITE;
      break;
  }
}

void print_direction(direction_t d) {
  static const char* names[] = {"NE", "NW", "E", "W", "SE", "SW"};
  printf("%s", names[d]);
}

int32_t parse_line(direction_t* restrict directions, const unsigned char* restrict s) {
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

static inline
int32_t
count_adjacent_black_tiles(color_t* restrict grid, int32_t x, int32_t y) {
  int32_t count = 0;
  int8_t parity = y & 1; // 1 for odd, 0 for even

  for (int8_t i=0; i < 6; i++) {
    int32_t dx = directions[parity][i][0];
    int32_t dy = directions[parity][i][1];
    count += grid[(y + dy) * GRIDSIZE + (x + dx)];
  }

  return count;
}

void apply_rules(grid_t* restrict grid, int32_t bounds[4]) {
  memcpy(grid->new_tiles, grid->tiles, GRIDSIZE_SQ * sizeof(color_t));

  for (int32_t y = bounds[2]; y <= bounds[3]; y++) {
    for (int32_t x = bounds[0]; x <= bounds[1]; x++) {
      int32_t black_neighbors = count_adjacent_black_tiles(grid->tiles, x, y);
      switch (grid->tiles[y * GRIDSIZE + x]) {
        case WHITE:
          if (black_neighbors == 2) {
            grid->new_tiles[y * GRIDSIZE + x] = BLACK;
            grid->black_tile_count++;

            if (x == bounds[1]) {
              bounds[1]++; 
            } else if (x == bounds[0]) {
              bounds[0]--;
            } 

            if (y == bounds[3]) {
              bounds[3]++;
            } else if (y == bounds[2]) {
              bounds[2]--;
            } 
          } 
          break;

        case BLACK:
          if (black_neighbors == 0 || black_neighbors > 2) {
            grid->new_tiles[y * GRIDSIZE + x] = WHITE;
            grid->black_tile_count--;
          } 
          break;
      }
    }
  } 

  // swap out pointers
  color_t* tmp = grid->tiles;
  grid->tiles = grid->new_tiles;
  grid->new_tiles = tmp;
}

int day24() {
  grid_t grid = {
    .tiles = (color_t*)calloc(GRIDSIZE_SQ * 2, sizeof(color_t)),
    .black_tile_count = 0,
  };
  if (!grid.tiles) {
    err(EXIT_FAILURE, "error allocating memory for grid tiles");
  }
  grid.new_tiles = &grid.tiles[GRIDSIZE_SQ];

  const char unsigned *s = input;
  direction_t* dir_instructions = (direction_t*)malloc(sizeof(direction_t) * 50);
  if (!dir_instructions) {
    err(EXIT_FAILURE, "error allocating memory for directions");
  }

  int32_t max_x = 0;
  int32_t max_y = 0;
  int32_t min_y = 1000;
  int32_t min_x = 1000;

  while (*s != '\0') {
    int32_t n = parse_line(dir_instructions, s);
    while (*s != '\n' && *s != '\0') s++;
    if (*s == '\n') s++;

    // go back to start tile (middle of grid)
    int32_t x = GRIDSIZE / 2;
    int32_t y = x;
   
    for (int8_t i = 0; i < n; i++) {
      int8_t parity = y & 1;
      int8_t dx = directions[parity][dir_instructions[i]][0];
      int8_t dy = directions[parity][dir_instructions[i]][1];
      x += dx;
      y += dy;
    }

    flip_tile(&grid.tiles[(y * GRIDSIZE) + x], &grid.black_tile_count);

    if (y < min_y) {
      min_y = y;
    } else if (y > max_y) {
      max_y = y;
    }

    if (x < min_x) {
      min_x = x;
    } else if (x > max_x) {
      max_x = x;
    }
  }

  printf("%d\n", grid.black_tile_count);
  assert(grid.black_tile_count == 244);

  int32_t bounds[4] = {min_x-1, max_x+1, min_y-1, max_y+1};
  for (int32_t i = 0; i < 100; i++) {
    apply_rules(&grid, bounds);
  }
  printf("%d\n", grid.black_tile_count);
  assert(grid.black_tile_count == 3665);

  free(dir_instructions);
  free(grid.tiles);
  return 0;
}
