#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "inputs/24.h"

typedef enum {
  WHITE = 0,
  BLACK = 1,
} color_t;

typedef enum { NE, NW, E, W, SE, SW } direction_t;

static const int32_t GRIDSIZE = 128;
static const int32_t GRIDSIZE_SQ = GRIDSIZE * GRIDSIZE;
int32_t black_tile_count = 0;

void flip_tile(color_t* restrict color) {
  switch (*color) {
    case WHITE:
      black_tile_count++;
      *color = BLACK;
      break;

    case BLACK:
      black_tile_count--;
      *color = WHITE;
      break;
  }
}

void print_direction(direction_t d) {
  static const char* names[] = {"NE", "NW", "E", "W", "SE", "SW"};
  printf("%s", names[d]);
}

int32_t parse_line(direction_t* restrict directions, const unsigned char* restrict line) {
  const unsigned char* s = line;
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

#define isodd(v) (v & 1)
#define iseven(v) (v & 1) == 0

int32_t
count_adjacent_black_tiles(color_t* restrict grid, int32_t x, int32_t y) {
  int32_t count = 0;
  static const int32_t directions[2][6][2] = {
    {
        // even
      { -1, -1 },
      { 0, -1 },
      { -1, 0 },
      { 1, 0 },
      { -1, 1 },
      { 0, 1 },
    },
    {
        // odd
      { 0, -1},
      { 1, -1},
      { -1, 0},
      { +1, 0},
      { 0, +1},
      { +1, +1},
    }
  };
  int32_t parity = y & 1; // 1 for odd, 0 for even

  for (int32_t i=0; i < 6; i++) {
    int32_t dx = directions[parity][i][0];
    int32_t dy = directions[parity][i][1];
    count += grid[(y + dy) * GRIDSIZE + (x + dx)];
  }

  return count;
}

void apply_rules(color_t* restrict grid, int32_t bounds[4]) {
  color_t new_grid[GRIDSIZE_SQ];
  memcpy(new_grid, grid, GRIDSIZE_SQ * sizeof(color_t));

  for (int32_t y = bounds[2]; y <= bounds[3]; y++) {
    for (int32_t x = bounds[0]; x <= bounds[1]; x++) {
      int32_t black_neighbors = count_adjacent_black_tiles(grid, x, y);
      color_t* color = &grid[y * GRIDSIZE + x];
      color_t* new_color = &new_grid[y * GRIDSIZE + x];
      switch (*color) {
        case WHITE:
          if (black_neighbors == 2) {
            *new_color = BLACK;
            black_tile_count++;

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
            *new_color = WHITE;
            black_tile_count--;
          } 
          break;
      }
    }
  }

  memcpy(grid, new_grid, GRIDSIZE_SQ * sizeof(color_t));
}

int day24() {
  black_tile_count = 0;
  color_t* grid = (color_t*)calloc(GRIDSIZE_SQ, sizeof(color_t));
  if (!grid) {
    return 1;
  }

  const char unsigned *s = input;
  direction_t* directions = (direction_t*)malloc(sizeof(direction_t) * 50);
  if (!directions) {
    err(EXIT_FAILURE, "error allocating memory for directions");
  }

  int32_t max_x = 0;
  int32_t max_y = 0;
  int32_t min_y = 1000;
  int32_t min_x = 1000;

  while (*s != '\0') {
    int32_t ndirections = parse_line(directions, s);
    while (*s != '\n' && *s != '\0') s++;
    if (*s == '\n') s++;

    // go back to start tile
    int32_t x = GRIDSIZE / 2;
    int32_t y = x;

   
    for (int32_t i = 0; i < ndirections; i++) {
      switch (directions[i]) {
        case E:
          x += 1;
          break;

        case W:
          x -= 1;
          break;

        case NE:
          if (isodd(y)) {
            x++;
          }
          y -= 1;
          break;

        case NW:
          if (iseven(y)) {
            x--;
          }
          y -= 1;
          break;

        case SE:
         if (isodd(y)) {
            x++;
            max_x = x > max_x ? x : max_x;
          }
          y += 1;
          break;

        case SW:
          if (iseven(y)) {
            x--;
          }
          y += 1;
          break;
      }
    }

    flip_tile(&grid[(y * GRIDSIZE) + x]);

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

  printf("%d\n", black_tile_count);
  assert(black_tile_count == 244);

  int32_t bounds[4] = {min_x-1, max_x+1, min_y-1, max_y+1};
  for (int32_t i = 0; i < 100; i++) {
    apply_rules(grid, bounds);
  }
  printf("%d\n", black_tile_count);
  assert(black_tile_count == 3665);

  free(directions);
  free(grid);
  return 0;
}
