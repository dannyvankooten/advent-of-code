#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 6: Chronal Coordinates"

struct Point {
  int x;
  int y;
};

void determine_bounds(struct Point *points, size_t n, int bounds[4]) {
  bounds[0] = points[0].x;
  bounds[1] = points[0].y;
  bounds[2] = points[0].x;
  bounds[3] = points[0].y;

  for (size_t i = 1; i < n; i++) {
    if (points[i].x < bounds[0]) {
      bounds[0] = points[i].x;
    } else if (points[i].x > bounds[2]) {
      bounds[2] = points[i].x;
    }

    if (points[i].y < bounds[1]) {
      bounds[1] = points[i].y;
    } else if (points[i].y > bounds[3]) {
      bounds[3] = points[i].y;
    }
  }
}

size_t parse(const char *s, struct Point *points, int bounds[4]) {
  size_t n = 0;

  while (*s != 0x0) {
    s = parse_int(&points[n].x, s);
    s = skip(", ", s);
    s = parse_int(&points[n].y, s);
    s = skip_optional('\n', s);

    n++;
  }

  determine_bounds(points, n, bounds);
  return n;
}

int get_closest_point(struct Point *points, size_t n, int x, int y) {
  int closest_idx = -1;
  int closest_distance = 1 << 30;

  for (size_t p = 0; p < n; p++) {
    int distance = abs(x - points[p].x) + abs(y - points[p].y);

    if (distance < closest_distance) {
      closest_distance = distance;
      closest_idx = (int)p;
    } else if (distance == closest_distance) {
      // if equally low, keep lowest distance but reset index
      closest_idx = -1;
    }
  }

  return closest_idx;
}

int pt1(struct Point points[], size_t n, int bounds[4]) {
  int areas[n];
  memset(areas, 0, n * sizeof(int));

  char infinite[n];
  memset(areas, 0, n * sizeof(*infinite));

  for (int x = bounds[0], y = bounds[1]; x <= bounds[2] && y <= bounds[3];) {
    int closest_idx = get_closest_point(points, n, x, y);
    if (closest_idx > -1) {
      areas[closest_idx]++;

      // if closest point touches any bound
      // it's guaranteed to be infinite
      if (x == bounds[0] || x == bounds[2] || y == bounds[1] ||
          y == bounds[3]) {
        infinite[closest_idx] = 1;
      }
    }

    // prepare for next iteration
    if (x >= bounds[2]) {
      x = bounds[0];
      y++;
    } else {
      x++;
    }
  }

  size_t largest = 0;
  for (size_t p = 0; p < n; p++) {
    if (areas[p] > areas[largest] && infinite[p] == 0) {
      largest = p;
    }
  }

  return areas[largest];
}

int pt2(struct Point points[], size_t n, int bounds[4]) {
  int count = 0;
  for (int x = bounds[0], y = bounds[1]; x <= bounds[2] && y <= bounds[3];) {
    int distance = 0;

    for (size_t p = 0; p < n && distance < 10000; p++) {
      struct Point point = points[p];
      distance += (abs(point.x - x) + abs(point.y - y));
    }

    if (distance < 10000) {
      count += 1;
    }

    // prepare for next iteration
    if (x >= bounds[2]) {
      x = bounds[0];
      y++;
    } else {
      x++;
    }
  }

  return count;
}

int main(void) {
  clock_t t = clock_time();
  char input[1024 * 64];
  read_input_file(input, 64 * 1024, "input.txt");

  int bounds[4];
  struct Point *points = malloc_or_die(50 * sizeof(*points));
  size_t n = parse(input, points, bounds);

  // printf("%ld coords: (%d, %d) to (%d, %d)\n", n, bounds[0], bounds[1],
  //        bounds[2], bounds[3]);

  int a1 = pt1(points, n, bounds);
  int a2 = pt2(points, n, bounds);

  free(points);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}

// part 1: 3890 CORRECT
// part 2: 99540 too high
// part 2: 40284 CORRECT
