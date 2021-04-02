#include <assert.h>
#include <err.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int8_t SQ_TREE = 1;
const int8_t SQ_OPEN = 0;
const int32_t HEIGHT = 323;
const int32_t WIDTH = 31;
uint64_t count_trees(int8_t* grid, int32_t slope_y, int32_t slope_x);

int day3() {
  FILE* f = fopen("inputs/03.input", "r");
  if (!f) {
    err(EXIT_FAILURE, "error reading input file");
  }
  char buf[BUFSIZ] = {0};
  int8_t* grid = calloc(HEIGHT * WIDTH, sizeof(int8_t));
  if (!grid) {
    err(EXIT_FAILURE, "error allocating memory for grid values");
  }

  int32_t y = 0;
  while (fgets(buf, BUFSIZ, f) != NULL) {
    int32_t x = 0;
    char* s = buf;
    while (*s != '\n' && *s != '\0') {
      grid[y * WIDTH + x] = (*s == '#') ? SQ_TREE : SQ_OPEN;
      s++;
      x++;
    }

    y++;
  }
  fclose(f);

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

  printf("%" PRId64 "\n", product);
  assert(product == 4355551200);
  free(grid);
  return 0;
}

uint64_t count_trees(int8_t* grid, int slope_y, int slope_x) {
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