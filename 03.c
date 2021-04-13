#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "inputs/03.h"

typedef enum {
  SQ_TREE = 1,
  SQ_OPEN = 0,
} square_t;

const int32_t HEIGHT = 323;
const int32_t WIDTH = 31;
static uint64_t count_trees(square_t* grid, int32_t slope_y, int32_t slope_x);

int day3() {
  const unsigned char *s = input;
  
  square_t* grid = calloc(HEIGHT * WIDTH, sizeof(square_t));
  if (!grid) {
    err(EXIT_FAILURE, "error allocating memory for grid values");
  }

  int32_t y = 0;
  while (*s != '\0') {
    int32_t x = 0;

    while (*s != '\n' && *s != '\0') {
      grid[y * WIDTH + x] = (*s == '#') ? SQ_TREE : SQ_OPEN;
      s++;
      x++;
    }

    if (*s == '\n') {
      s++;
      y++;
    }
  }

  // // print grid
  // for (int y=0; y < HEIGHT; y++) {
  //     for (int x=0; x < WIDTH; x++) {
  //         printf("%c", grid[y][x] == SQ_OPEN ? '.' : '#');
  //     }
  //     printf("\n");
  // }

  // last x, y
  // printf("\n\nLast: %d\n", grid[(HEIGHT-1) * WIDTH + WIDTH-1]);

  int64_t product = count_trees(grid, 1, 1) * count_trees(grid, 1, 3) *
                    count_trees(grid, 1, 5) * count_trees(grid, 1, 7) *
                    count_trees(grid, 2, 1);

  printf("%ld\n", product);
  assert(product == 4355551200);
  free(grid);
  return 0;
}

static 
uint64_t count_trees(square_t* grid, int slope_y, int slope_x) {
  uint64_t tree_count = 0;
  for (int32_t y = 0, x = 0; y < 322; y += slope_y, x += slope_x) {
    if (x >= 31) {
      x -= 31;
    }

    if (grid[y * WIDTH + x] == SQ_TREE) {
      tree_count++;
    }
  }
  return tree_count;
}