#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputs/11.h"

enum position {
  POS_FLOOR = -1,
  POS_EMPTY_SEAT = 0,
  POS_OCCUPIED_SEAT = 1,
};

typedef struct seat seat_t;
struct seat {
  int index;
  int neighbors[8];
  int nneighbors;
};

struct grid {
  int occupied_seat_count;
  int nseats;
  int width;
  int height;
  seat_t *seats;
  enum position* values;
  enum position* new_values;
};
typedef struct grid grid_t;

#define get_grid_value_by_coords(grid, x, y) grid->values[y * grid->width + x]

static void
collect_neighbors(grid_t* restrict grid, seat_t* restrict seat) {
  static const int directions[][2] = {
      {0, 1},    // right
      {0, -1},   // left
      {1, 0},    // down
      {-1, 0},   // up
      {1, 1},    // down-right
      {-1, 1},   // up-right,
      {1, -1},   // down-left
      {-1, -1},  // up-left
  };
  const int pos_x = seat->index % grid->width;
  const int pos_y = (seat->index - pos_x) / grid->width;

  for (int d = 0; d < 8; d++) {
    int dy = directions[d][0];
    int dx = directions[d][1];
    for (int y = pos_y + dy, x = pos_x + dx;
         y >= 0 && x >= 0 && x < grid->width && y < grid->height; x += dx, y += dy) {
      if (get_grid_value_by_coords(grid, x, y) != POS_FLOOR) {
        seat->neighbors[seat->nneighbors++] = y * grid->width + x;
        break;
      }
    }
  }
}

static void
print_grid(grid_t* restrict grid) {
    for (int y=0; y < grid->height; y++) {
        for (int x=0; x < grid->width; x++) {
            switch (get_grid_value_by_coords(grid, x, y)) {
                case POS_EMPTY_SEAT:
                    printf("L");
                break;

                case POS_FLOOR:
                    printf(".");
                break;

                case POS_OCCUPIED_SEAT:
                    printf("#");
                break;
            }
        }
        printf("\n");
    }
}

static int
count_occupied_neighbors(grid_t* restrict grid, int* restrict neighbors, int nneighbors) {
  int count = 0;
  for (int i=0; i < nneighbors; i++) {
    if (grid->values[neighbors[i]] == POS_OCCUPIED_SEAT) {
      count++;
    }
  }
  return count;
}

/*
A seat is permanently occupied if it has more than 8-n (n=5 for part 2) permanently empty neighbours and no permanently occupied neighbours yet.
It is permanently empty if it has a permanently occupied neighbour. :)
*/
static bool
transmute_grid(grid_t* restrict grid) {
  bool changed = false;
  memcpy(grid->new_values, grid->values, grid->width * grid->height * sizeof(enum position));

  for (int i=grid->nseats-1; i >= 0; i--) {
    seat_t* seat = &grid->seats[i];
    const int occupied_count = count_occupied_neighbors(grid, seat->neighbors, seat->nneighbors);
    enum position state = grid->values[seat->index];
    if (state == POS_OCCUPIED_SEAT) {
        if (occupied_count >= 5) {
          grid->new_values[seat->index] = POS_EMPTY_SEAT;
          grid->occupied_seat_count--;
          changed = true;
        } else {
            // permanently seated, remove from list
            grid->seats[i] = grid->seats[grid->nseats-1];
            grid->nseats--;
        }
    } else {
        if (occupied_count == 0) {
          grid->new_values[seat->index] = POS_OCCUPIED_SEAT;
          grid->occupied_seat_count++;
          changed = true;
        } else {
          // permanently empty, remove from list
          grid->seats[i] = grid->seats[grid->nseats-1];
          grid->nseats--;
        }
    }
  }

  // swap out grid with new grid
  enum position* tmp = grid->values;
  grid->values = grid->new_values;
  grid->new_values = tmp;
  return changed;
}

static void
parse_input(grid_t* restrict grid) {
  const unsigned char *s = input;

  // parse width
  while (s[grid->width] != '\n') grid->width++;

  // parse height
  while (*s != 0x0) {
    if (*s == '\n') {
      grid->height++;
    }

    s++;
  }
  s = input;

  // allocate memory for values (twice, for swapping out grid)
  grid->values = (enum position *) malloc(grid->width * grid->height * 2 * sizeof(enum position));
  if (!grid->values) {
    err(EXIT_FAILURE, "error allocating memory for grid values");
  }
  grid->new_values = &grid->values[grid->width * grid->height];
  grid->seats = malloc(grid->width * grid->height * sizeof(seat_t));
  grid->nseats = 0;

  int y = 0;
  while (*s != '\0') {
    for (int x = 0; *s != '\n' && *s != '\0'; s++, x++) {
      const enum position v = (*s == '.') ? POS_FLOOR : POS_EMPTY_SEAT;
      grid->values[y * grid->width + x] = v;
      grid->new_values[y * grid->width + x] = v;

      // if this is a seat, add it to list of seats
      if (v != POS_FLOOR) {
        seat_t *seat = &grid->seats[grid->nseats++];
        seat->nneighbors = 0;
        seat->index = y * grid->width + x;
      }
    }

    y++;

    if (*s == '\n') {
      s++;
    }
  }

  assert(y == grid->height);

  for (int i=0; i < grid->nseats; i++) {
    collect_neighbors(grid, &grid->seats[i]);
  }
}

int day11() {
  grid_t grid = {
      .height = 0,
      .width = 0,
      .occupied_seat_count = 0,
  };
  parse_input(&grid);

  // transmute grid until stable
  while (transmute_grid(&grid));

  // count occupied seats
  printf("%d\n", grid.occupied_seat_count);
  assert(grid.occupied_seat_count == 1990);

  free(grid.seats);
  free(grid.values);
  return 0;
}
