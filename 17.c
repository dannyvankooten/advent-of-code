#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputs/17.h"

#define GRIDSIZE 24

typedef enum {
  STATE_ACTIVE = 1,
  STATE_INACTIVE = 0,
} state_t;

struct grid {
  int8_t* neighbor_counts;
  char* values;
  char *new_values;
};
typedef struct grid grid_t;

grid_t read_input() {
  const unsigned char *s = input;

  // assume high enough values to fit our infinite grid
  grid_t g;
  g.values = (char*)calloc(GRIDSIZE * GRIDSIZE * GRIDSIZE * GRIDSIZE * 2, sizeof(char));
  if (!g.values) {
    err(EXIT_FAILURE, "could not allocate grid values");
  }
  g.new_values = &g.values[GRIDSIZE * GRIDSIZE * GRIDSIZE * GRIDSIZE];
  g.neighbor_counts =
      (int8_t*)malloc(GRIDSIZE * GRIDSIZE * GRIDSIZE * GRIDSIZE * sizeof(int8_t));
  if (!g.neighbor_counts) {
    err(EXIT_FAILURE, "could not allocate neighbor counts");
  }

  int32_t c = (GRIDSIZE / 2) - 1;  // centre point
  int32_t x;
  int32_t y = c;
  int32_t z = c;
  int32_t w = c;
  int32_t offset_2d = (w * GRIDSIZE * GRIDSIZE * GRIDSIZE) 
            + (z * GRIDSIZE * GRIDSIZE);

  while (*s != '\0') {
    x = c;
    while (*s != '\n' && *s != '\0') {
      g.values[offset_2d
            + (y * GRIDSIZE) + x] =
          (*s++ == '#') ? STATE_ACTIVE : STATE_INACTIVE;
      x++;
    }

    if (*s == '\n') {
      s++;
    }

    y++;
  }

  return g;
}

void print_grid(grid_t g) {
  int32_t count = 0;
  for (int32_t z = 0; z < GRIDSIZE; z++) {
    printf("z=%d\n", z - 100 / 2 - 2);
    for (int32_t y = 0; y < GRIDSIZE; y++) {
      for (int32_t x = 0; x < GRIDSIZE; x++) {
        if (g.values[(GRIDSIZE * GRIDSIZE * z) + (GRIDSIZE * y) + x] ==
            STATE_ACTIVE) {
          printf("#");
          count++;
        } else {
          printf(".");
        }
      }
      printf("\n");
    }
    printf("\n");
  }

  printf("count = %d\n", count);
}

void add_one_to_all_neighbours(grid_t* g,
                               int32_t pos_x,
                               int32_t pos_y,
                               int32_t pos_z,
                               int32_t pos_w) {
  int32_t idx_self = (pos_w * GRIDSIZE * GRIDSIZE * GRIDSIZE) 
                      + (pos_z * GRIDSIZE * GRIDSIZE) 
                      + (pos_y * GRIDSIZE) 
                      + pos_x;
  for (int8_t w = pos_w - 1; w <= pos_w + 1; w++) {
    for (int8_t z = pos_z - 1; z <= pos_z + 1; z++) {
      for (int8_t y = pos_y - 1; y <= pos_y + 1; y++) {
        for (int8_t x = pos_x - 1; x <= pos_x + 1; x++) {
          int32_t idx = (w * GRIDSIZE * GRIDSIZE * GRIDSIZE) 
            + (z * GRIDSIZE * GRIDSIZE) 
            + (y * GRIDSIZE) 
            + x;

          if (idx != idx_self) {
            g->neighbor_counts[idx] += 1;;
          }          
        }
      }
    }
  }
}

void update_neighbor_counts(grid_t* g) {
  memset(g->neighbor_counts, 0,
         GRIDSIZE * GRIDSIZE * GRIDSIZE * GRIDSIZE * sizeof(int8_t));
  for (int8_t w = 1; w < GRIDSIZE - 1; w++) {
    for (int8_t z = 1; z < GRIDSIZE - 1; z++) {
      for (int8_t y = 1; y < GRIDSIZE - 1; y++) {
        for (int8_t x = 1; x < GRIDSIZE - 1; x++) {
          int32_t idx = (w * GRIDSIZE * GRIDSIZE * GRIDSIZE) 
            + (z * GRIDSIZE * GRIDSIZE) 
            + (y * GRIDSIZE) 
            + x;
          if (g->values[idx] == STATE_ACTIVE) {
            // add one to all neighbors
            add_one_to_all_neighbours(g, x, y, z, w);
          }
        }
      }
    }
  }
}

int32_t transmute_grid(grid_t* g) {
  update_neighbor_counts(g);
  memset(g->new_values, STATE_INACTIVE, GRIDSIZE * GRIDSIZE * GRIDSIZE * GRIDSIZE * sizeof(char));
  int32_t count = 0;
  for (int32_t w = 1; w < GRIDSIZE - 1; w++) {
    for (int32_t z = 1; z < GRIDSIZE - 1; z++) {
      for (int32_t y = 1; y < GRIDSIZE - 1; y++) {
        for (int32_t x = 1; x < GRIDSIZE - 1; x++) {
          int32_t idx = (w * GRIDSIZE * GRIDSIZE * GRIDSIZE) 
              + (z * GRIDSIZE * GRIDSIZE) 
              + (y * GRIDSIZE) 
              + x;

          // check early to prevent following a pointer into the values grid below
          int8_t active_neighbor_count = g->neighbor_counts[idx];
          if (active_neighbor_count < 2 || active_neighbor_count > 3) {
            continue;
          }

          switch (g->values[idx]) {
            case STATE_ACTIVE:
              // (active_neighbor_count == 2 || active_neighbor_count == 3)
              g->new_values[idx] = STATE_ACTIVE;
              count++;
              break;

            default:
            case STATE_INACTIVE:
              // If a cube is inactive but exactly 3 of its neighbors
              // are active, the cube becomes active.
              if (active_neighbor_count == 3) {
                g->new_values[idx] = STATE_ACTIVE;
                count++;
              }
              break;
          }
        }
      }
    }
  }

  // swap out pointers
  char *tmp = g->values;
  g->values = g->new_values;
  g->new_values = tmp;
  return count;
}

// TODO: Grow grid dynamically, as this should also improve performance by
// reducing loop iterations
int day17() {
  grid_t g = read_input();
  int32_t count = 0;
  for (int32_t i = 0; i < 6; i++) {
    count = transmute_grid(&g);

#ifdef STEP
    getchar();
    printf("Step %d\n", i + 1);
    print_grid(g);
#endif
  }

  printf("%d\n", count);
  assert(count == 1380);
  free(g.neighbor_counts);
  free(g.values);
  return 0;
}
