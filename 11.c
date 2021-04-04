#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputs/11.h"

enum position {
  POS_FLOOR = -1,
  POS_EMPTY_SEAT = 0,
  POS_OCCUPIED_SEAT = 1,
};

struct grid {
  int32_t w;
  int32_t h;
  enum position* values;
  enum position* alt_values;
  bool stable;
  int32_t occupied_seat_count;
};
typedef struct grid grid_t;

int32_t count_occupied_adjacent_seats(grid_t* grid,
                                      int32_t pos_y,
                                      int32_t pos_x) {
  int32_t y_start = pos_y - 1;
  y_start = y_start < 0 ? 0 : y_start;
  int32_t y_end = pos_y + 1;
  y_end = y_end > grid->h - 1 ? grid->h - 1 : y_end;
  int32_t x_start = pos_x - 1 < 0 ? 0 : pos_x - 1;
  int32_t x_end = pos_x + 1 > grid->w - 1 ? grid->w - 1 : pos_x + 1;

  int32_t count = 0;
  for (int32_t y = y_start; y <= y_end; y++) {
    for (int32_t x = x_start; x <= x_end; x++) {
      if (y == pos_y && x == pos_x) {
        continue;
      }
      enum position v = grid->values[y * grid->w + x];
      if (v == POS_OCCUPIED_SEAT) {
        count++;
      }
    }
  }
  return count;
}

#define get_grid_value_by_coords(grid, x, y) grid->values[y * grid->w + x]

int32_t count_occupied_seats_in_los(grid_t* grid, int pos_y, int pos_x) {
  static const int8_t directions[8][2] = {
      {0, 1},    // right
      {0, -1},   // left
      {1, 0},    // down
      {-1, 0},   // up
      {1, 1},    // down-right
      {-1, 1},   // up-right,
      {1, -1},   // down-left
      {-1, -1},  // up-left
  };
  int32_t count = 0;

  for (int_fast8_t d = 0; d < 8; d++) {
    int_fast8_t dy = directions[d][0];
    int_fast8_t dx = directions[d][1];
    for (int32_t y = pos_y + dy, x = pos_x + dx;
         y >= 0 && x >= 0 && x < grid->w && y < grid->h; x += dx, y += dy) {
      enum position value = get_grid_value_by_coords(grid, x, y);
      if (value != POS_FLOOR) {
        count += value;
        break;
      }
    }
  }

  return count;
}

// static void print_grid(grid_t *grid) {
//     for (int32_t y=0; y < grid->h; y++) {
//         for (int32_t x=0; x < grid->w; x++) {
//             switch (get_grid_value_by_coords(grid, x, y)) {
//                 case POS_EMPTY_SEAT:
//                     printf("L");
//                 break;

//                 case POS_FLOOR:
//                     printf(".");
//                 break;

//                 case POS_OCCUPIED_SEAT:
//                     printf("#");
//                 break;
//             }
//         }
//         printf("\n");
//     }
// }

static void transmute_grid(grid_t* restrict grid) {
  grid->stable = true;
  for (int32_t y = 0; y < grid->h; y++) {
    for (int32_t x = 0; x < grid->w; x++) {
      enum position value = grid->values[(y * grid->w) + x];
      enum position* new_value = &grid->alt_values[(y * grid->w) + x];
      switch (value) {
        case POS_EMPTY_SEAT:
          if (count_occupied_seats_in_los(grid, y, x) == 0) {
            *new_value = POS_OCCUPIED_SEAT;
            grid->occupied_seat_count++;
            grid->stable = false;
          } else {
            *new_value = POS_EMPTY_SEAT;
          }
          break;

        case POS_OCCUPIED_SEAT:
          if (count_occupied_seats_in_los(grid, y, x) >= 5) {
            *new_value = POS_EMPTY_SEAT;
            grid->stable = false;
            grid->occupied_seat_count--;
          } else {
            *new_value = POS_OCCUPIED_SEAT;
          }
          break;

        case POS_FLOOR:
          // Floor never changes
          *new_value = POS_FLOOR;
          break;
      }
    }
  }

  // swap out grid with new grid
  enum position* tmp = grid->values;
  grid->values = grid->alt_values;
  grid->alt_values = tmp;
}

static 
void 
parse_input(grid_t* restrict grid) {
  const unsigned char *s = input;
  
  // parse width
  while(s[grid->w] != '\n') grid->w++;
  
  // allocate memory for values
  grid->values = (enum position *) malloc(grid->w * 100 * sizeof(enum position));
  if (!grid->values) {
    err(EXIT_FAILURE, "error allocating memory for grid values");
  }

  while (*s != '\0') {
    for (int32_t x = 0; *s != '\n' && *s != '\0'; s++, x++) {
      grid->values[grid->h * grid->w + x] = (*s == '.') ? POS_FLOOR : POS_EMPTY_SEAT;
    }
    grid->h++;

    if (*s == '\n') s++;
  }

  grid->alt_values = (enum position *) malloc(grid->w * 100 * sizeof(enum position));
  if (!grid->alt_values) {
    err(EXIT_FAILURE, "error allocating memory for alt grid values");
  }

}

int day11() {
  grid_t grid = {
      .h = 0,
      .w = 0,
      .stable = 0,
      .occupied_seat_count = 0,
  };
  parse_input(&grid);

  while (!grid.stable) {
    transmute_grid(&grid);
  }

  // count occupied seats
  printf("%d\n", grid.occupied_seat_count);
  assert(grid.occupied_seat_count == 1990);

  free(grid.values);
  return 0;
}
