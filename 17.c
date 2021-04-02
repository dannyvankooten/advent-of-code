#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  STATE_ACTIVE = 1,
  STATE_INACTIVE = 0,
};

struct grid {
  int32_t size;
  char* values;
  short* neighbor_counts;
};
typedef struct grid grid_t;

grid_t read_input() {
  char linebuf[BUFSIZ] = {0};
  FILE* f = fopen("inputs/17.txt", "r");
  if (!f)
    err(EXIT_FAILURE, "error reading input file");

  // assume high enough values to fit our infinite grid
  grid_t g = {
      .size = 21,
  };
  g.values = (char*)calloc(g.size * g.size * g.size * g.size, sizeof(char));
  if (!g.values) {
    err(EXIT_FAILURE, "could not allocate grid values");
  }
  g.neighbor_counts =
      (short*)malloc(g.size * g.size * g.size * g.size * sizeof(short));
  if (!g.neighbor_counts) {
    err(EXIT_FAILURE, "could not allocate neighbor counts");
  }

  int32_t c = g.size / 2 - 2;  // centre point
  int32_t x;
  int32_t y = c;
  int32_t z = c;
  int32_t w = c;
  while (fgets(linebuf, BUFSIZ, f) != NULL) {
    x = c;
    char* s = linebuf;
    while (*s != '\n' && *s != '\0') {
      g.values[(w * g.size * g.size * g.size) + (z * g.size * g.size) +
               (g.size * y) + x] =
          (*s++ == '#') ? STATE_ACTIVE : STATE_INACTIVE;
      x++;
    }
    y++;
  }
  fclose(f);

  return g;
}

void print_grid(grid_t g) {
  int32_t count = 0;
  for (int32_t z = 0; z < g.size; z++) {
    printf("z=%d\n", z - 100 / 2 - 2);
    for (int32_t y = 0; y < g.size; y++) {
      for (int32_t x = 0; x < g.size; x++) {
        if (g.values[(g.size * g.size * z) + (g.size * y) + x] ==
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
  int32_t idx;
  int32_t idx_self = (pos_w * g->size * g->size * g->size) +
                     (g->size * g->size * pos_z) + (g->size * pos_y) + pos_x;
  ;
  for (int32_t w = pos_w - 1; w <= pos_w + 1; w++) {
    for (int32_t z = pos_z - 1; z <= pos_z + 1; z++) {
      for (int32_t y = pos_y - 1; y <= pos_y + 1; y++) {
        for (int32_t x = pos_x - 1; x <= pos_x + 1; x++) {
          idx = (w * g->size * g->size * g->size) + (g->size * g->size * z) +
                (g->size * y) + x;
          if (idx == idx_self) {
            continue;
          }

          g->neighbor_counts[idx] += 1;
        }
      }
    }
  }
}

void update_neighbor_counts(grid_t* g) {
  memset(g->neighbor_counts, 0,
         g->size * g->size * g->size * g->size * sizeof(short));
  int32_t idx;
  for (int32_t w = 1; w < g->size - 1; w++) {
    for (int32_t z = 1; z < g->size - 1; z++) {
      for (int32_t y = 1; y < g->size - 1; y++) {
        for (int32_t x = 1; x < g->size - 1; x++) {
          idx = (w * g->size * g->size * g->size) + (g->size * g->size * z) +
                (g->size * y) + x;
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
  char* values =
      (char*)calloc(g->size * g->size * g->size * g->size, sizeof(char));
  if (!values) {
    err(EXIT_FAILURE, "error allocating memory for new grid values");
  }

  update_neighbor_counts(g);
  int32_t idx;
  int32_t active_neighbor_count;
  int32_t count = 0;
  for (int32_t w = 1; w < g->size - 1; w++) {
    for (int32_t z = 1; z < g->size - 1; z++) {
      for (int32_t y = 1; y < g->size - 1; y++) {
        for (int32_t x = 1; x < g->size - 1; x++) {
          idx = (w * g->size * g->size * g->size) + (g->size * g->size * z) +
                (g->size * y) + x;
          active_neighbor_count = g->neighbor_counts[idx];
          switch (g->values[idx]) {
            case STATE_ACTIVE:
              if (active_neighbor_count == 2 || active_neighbor_count == 3) {
                values[idx] = STATE_ACTIVE;
                count++;
              }
              break;

            default:
            case STATE_INACTIVE:
              // If a cube is inactive but exactly 3 of its neighbors
              // are active, the cube becomes active.
              if (active_neighbor_count == 3) {
                values[idx] = STATE_ACTIVE;
                count++;
              }
              break;
          }
        }
      }
    }
  }
  free(g->values);
  g->values = values;
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
  free(g.neighbor_counts);
  free(g.values);
  return 0;
}
