#include "adventofcode.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 17: Reservoir Research"
#define N 2048

struct Rect {
  unsigned int x1;
  unsigned int x2;
  unsigned int y1;
  unsigned int y2;
};

struct Point {
  unsigned int x;
  unsigned int y;
};

static inline void swap(unsigned int *a, unsigned int *b) {
  const unsigned int tmp = *b;
  *b = *a;
  *a = tmp;
}

struct Stack {
  struct Point *elements;
  size_t cap;
  size_t size;
};

static struct Stack stack_init(void) {
  struct Stack s;
  s.cap = 1024;
  s.size = 0;
  s.elements = malloc(s.cap * sizeof(*s.elements));
  return s;
}

static void stack_destroy(struct Stack *s) { free(s->elements); }

static void stack_push(struct Stack *s, struct Point e) {
  if (s->size == s->cap) {
    s->cap *= 2;
    s->elements = realloc(s->elements, s->cap * sizeof(*s->elements));
  }

  s->elements[s->size++] = e;
}

static struct Point stack_pop(struct Stack *s) {
  assert(s->size > 0);
  return s->elements[--s->size];
}

static struct Rect parse_rect(const char buf[BUFSIZ]) {
  struct Rect r;

  r.x1 = (unsigned) atoi(&buf[2]);
  r.x2 = r.x1;

  char *s = strchr(&buf[2], '=');
  assert(s != NULL);
  r.y1 =  (unsigned) atoi(s + 1);

  s = strstr(s, "..");
  assert(s != NULL);
  r.y2 =  (unsigned)  atoi(s + 2);

  if (buf[0] == 'y') {
    swap(&r.x1, &r.y1);
    swap(&r.x2, &r.y2);
  }

  // printf("Parsed %d, %d -> %d, %d\n", r.x1, r.y1, r.x2, r.y2);
  return r;
}

static struct Rect parse_input(char grid[N][N]) {
  struct Rect b;
  b.x1 = UINT_MAX;
  b.x2 = 0;
  b.y1 = UINT_MAX;
  b.y2 = 0;

  memset(grid, '.', N * N * sizeof(char));
  char buf[BUFSIZ];

  while (fgets(buf, BUFSIZ, stdin)) {
    struct Rect r = parse_rect(buf);

    // update bounds
    if (r.x1 < b.x1) {
      b.x1 = r.x1;
    }
    if (r.x2 > b.x2) {
      b.x2 = r.x2;
    }
    if (r.y1 < b.y1) {
      b.y1 = r.y1;
    }
    if (r.y2 > b.y2) {
      b.y2 = r.y2;
    }

    for (unsigned int y = r.y1; y <= r.y2; y++) {
      for (unsigned int x = r.x1; x <= r.x2; x++) {
        grid[y][x] = '#';
      }
    }
  }

  return b;
}

static char is_supported(char grid[N][N], const struct Rect *bounds,
                         const unsigned int x, const unsigned int y) {

  const unsigned int x_min = bounds->x1 - 2;
  const unsigned int x_max = bounds->x2 + 2;

  for (unsigned int x2 = x - 1;
       x2 >= x_min && x2 <= x_max + 2 && grid[y][x2] != '#';
       x2--) {
    if (grid[y + 1][x2] != '#' && grid[y + 1][x2] != '~') {
      return 0;
    }
  }

  for (unsigned int x2 = x + 1;
       x2 >= x_min && x2 <= x_max + 2 && grid[y][x2] != '#';
       x2++) {
    if (grid[y + 1][x2] != '#' && grid[y + 1][x2] != '~') {
      return 0;
    }
  }

  return 1;
}

static void set(char grid[N][N], const struct Rect *b, const unsigned int x,
                const unsigned int y, const char v, struct Stack *stack) {
  if (y > b->y2 || x < b->x1-1 || x > b->x2+1 || y < b->y1) {
    return;
  }

  if (grid[y][x] == '#' || grid[y][x] == v) {
    return;
  }

  grid[y][x] = v;

  stack_push(stack, (struct Point){x, y});
  if (x < b->x2) stack_push(stack, (struct Point){x + 1, y});
  if (y < b->y2) stack_push(stack, (struct Point){x, y + 1});
  if (x > b->x1) stack_push(stack, (struct Point){x - 1, y});
  if (y > b->y1) stack_push(stack, (struct Point){x, y - 1});
}

int main(void) {
  clock_t t = clock_time();
  char grid[N][N];
  struct Rect b = parse_input(grid);
  struct Stack stack = stack_init();

  // start at tile within y-range below source (500, 0)
  set(grid, &b, 500, b.y1, '|', &stack);
  while (stack.size > 0) {
    const struct Point at = stack_pop(&stack);
    const char *t = &grid[at.y][at.x];
    if (*t == '#' || *t == '.') {
      continue;
    }

    // add falling water below any other falling water
    if (*t == '|' && grid[at.y + 1][at.x] == '.') {
      set(grid, &b, at.x, at.y + 1, '|', &stack);
    }

    // supported falling water becomes at rest
    if (*t == '|' && is_supported(grid, &b, at.x, at.y)) {
      set(grid, &b, at.x, at.y, '~', &stack);
    }

    // water spreads left and right if not able to go down
    if (*t == '|' && (grid[at.y + 1][at.x] == '~' || grid[at.y + 1][at.x] == '#')) {
      set(grid, &b, at.x - 1, at.y, '|', &stack);
      set(grid, &b, at.x + 1, at.y, '|', &stack);
    }

    // water at rest spreads out
    if (*t == '~') {
      set(grid, &b, at.x - 1, at.y, '~', &stack);
      set(grid, &b, at.x + 1, at.y, '~', &stack);
    }
  }
  stack_destroy(&stack);

  unsigned int pt1 = 0;
  unsigned int pt2 = 0;
  for (unsigned int y = b.y1; y <= b.y2; y++) {
    for (unsigned int x = b.x1-1; x <= b.x2+1; x++) {
      if (grid[y][x] == '|' || grid[y][x] == '~') {
        pt1++;
      }

      if ( grid[y][x] == '~') {
        pt2++;
      }
    }
  }

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", pt1);
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
