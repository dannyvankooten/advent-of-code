#include "adventofcode.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 13: Mine Cart Madness"

struct Point {
  int x;
  int y;
};

enum {
  NORTH = 0,
  EAST,
  SOUTH,
  WEST,
};

enum TURN {
  LEFT = -1,
  STRAIGHT = 0,
  RIGHT = 1,
};

static const struct Point directions[4] = {
    [NORTH] = {0, -1},
    [EAST] = {1, 0},
    [SOUTH] = {0, 1},
    [WEST] = {-1, 0},
};

struct Cart {
  struct Point pos;

  // offset into directions array
  unsigned char d;

  // offset to apply to d at every intersection (+) tile
  // either -1 (left), 0 (straight) or 1 (right)
  // increments by 1 after every intersection
  char nd;

  // boolean indicating whether this cart has crashed
  char crashed;
};

static int qsort_cmp_carts(const void *a, const void *b) {
  const struct Cart *ca = (const struct Cart *)a;
  const struct Cart *cb = (const struct Cart *)b;

  if (ca->pos.y == cb->pos.y) {
    return (ca->pos.x > cb->pos.x) - (ca->pos.x < cb->pos.x);
  }

  return (ca->pos.y > cb->pos.y) - (ca->pos.y < cb->pos.y);
}

static void parse_input(char grid[150][150], unsigned int *height,
                        unsigned int *width) {
  memset(grid, ' ', 150 * 150 * sizeof(char));
  char buf[256];
  unsigned line_width = 0;
  *height = 0;
  *width = 0;

  while (fgets(buf, 256, stdin) && *height < 150) {
    line_width = (unsigned int)(strlen(buf) - 1);
    *width = line_width > *width ? line_width : *width;
    assert(*width <= 150);
    memcpy(grid[*height], buf, line_width * sizeof(char));
    *height += 1;
  }
}

static void carts_init(char grid[150][150], const unsigned height,
                       const unsigned width, struct Cart carts[],
                       unsigned int *ncarts) {
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x < width; x++) {
      char *t = &grid[y][x];
      if (*t != '>' && *t != '<' && *t != '^' && *t != 'v') {
        continue;
      }

      struct Cart c;
      c.nd = LEFT;
      c.crashed = 0;
      c.pos.x = (int)x;
      c.pos.y = (int)y;
      switch (*t) {
      case '^':
        c.d = NORTH;
        *t = '|';
        break;
      case '>':
        c.d = EAST;
        *t = '-';
        break;
      case 'v':
        c.d = SOUTH;
        *t = '|';
        break;
      case '<':
        c.d = WEST;
        *t = '-';
        break;
      }

      carts[*ncarts] = c;
      *ncarts += 1;

      assert(*ncarts < 32);
    }
  }
}

static void cart_step(char grid[150][150], const unsigned height,
                      const unsigned width, struct Cart *c) {
  // apply the offset
  struct Point v = directions[c->d];
  c->pos.x += v.x;
  c->pos.y += v.y;
  assert(c->pos.x >= 0 && c->pos.y >= 0);
  assert((unsigned int)c->pos.x < width);
  assert((unsigned int)c->pos.y < height);

   // if cart is at intersection OR curve
  const char t = grid[c->pos.y][c->pos.x];
  assert(t != ' ');
  if (t == '+') {
    if (c->nd == LEFT) {
      c->d = c->d == NORTH ? WEST : c->d - 1;
    } else if (c->nd == RIGHT) {
      c->d = c->d == WEST ? NORTH : c->d + 1;
    }

    // update direction for next intersection
    if (++c->nd == 2) {
      c->nd = LEFT;
    }
  } else if (t == '/') {
    switch (c->d) {
    case NORTH:
      c->d = EAST;
      break;
    case EAST:
      c->d = NORTH;
      break;
    case SOUTH:
      c->d = WEST;
      break;
    case WEST:
      c->d = SOUTH;
      break;
    }
  } else if (t == '\\') {
    switch (c->d) {
    case NORTH:
      c->d = WEST;
      break;
    case EAST:
      c->d = SOUTH;
      break;
    case SOUTH:
      c->d = EAST;
      break;
    case WEST:
      c->d = NORTH;
      break;
    }
  }
}

int main(void) {
  clock_t t = clock_time();
  char grid[150][150];
  unsigned height;
  unsigned width;
  parse_input(grid, &height, &width);

  unsigned int ncarts = 0;
  struct Cart carts[32];
  carts_init(grid, height, width, carts, &ncarts);

  struct Point first_crash = {-1, -1};
  struct Point last_cart;

  for (;;) {
    // move carts
    for (unsigned int c = 0; c < ncarts; c++) {
      // skip crashed carts
      if (carts[c].crashed == 1) {
        continue;
      }

      // apply the offset
      cart_step(grid, height, width, &carts[c]);

      // check for collission
      for (unsigned int cb = 0; cb < ncarts; cb++) {
        if (c == cb || carts[cb].crashed == 1) {
          continue;
        }

        if (carts[c].pos.x == carts[cb].pos.x &&
            carts[c].pos.y == carts[cb].pos.y) {
          if (first_crash.x == -1 && first_crash.y == -1) {
            first_crash = carts[c].pos;
          }

          // mark carts as crashed
          carts[c].crashed = 1;
          carts[cb].crashed = 1;
        }
      }
    }

    // sort carts by y, x position
    qsort(carts, ncarts, sizeof(*carts), qsort_cmp_carts);

    // count carts still in the game
    int cnt = 0;
    for (unsigned c = 0; c < ncarts && cnt < 2; c++) {
      if (carts[c].crashed == 0) {
        cnt++;
        last_cart = carts[c].pos;
      }
    }

    if (cnt == 1) {
      break;
    }
  }

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d,%d\n", first_crash.x, first_crash.y);
  printf("Part 2: %d,%d\n", last_cart.x, last_cart.y);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
