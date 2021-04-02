#include <assert.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct instruction {
  char action;
  int32_t value;
};

struct position {
  int32_t x;
  int32_t y;
};

struct ship {
  struct position pos;
  struct position waypoint;
};

void move_waypoint(struct ship* ship, char dir, int32_t value) {
  switch (dir) {
    case 'N':
      ship->waypoint.y += value;
      break;
    case 'E':
      ship->waypoint.x += value;
      break;
    case 'S':
      ship->waypoint.y -= value;
      break;
    case 'W':
      ship->waypoint.x -= value;
      break;
  }
}
#define M_PI 3.14159265358979323846 /* pi */

void rotate_waypoint(struct ship* ship, int value) {
  double v = (double)value * M_PI / 180.0;
  int32_t x1 = round(ship->waypoint.x * cos(v) - ship->waypoint.y * sin(v));
  int32_t y1 = round(ship->waypoint.x * sin(v) + ship->waypoint.y * cos(v));
  ship->waypoint.x = x1;
  ship->waypoint.y = y1;
}

int day12() {
  int32_t instructions_n = 0;
  struct instruction* instructions = malloc(800 * sizeof(struct instruction));
  if (!instructions) {
    err(EXIT_FAILURE, "error allocating memory for instructions");
  }
  FILE* f = fopen("inputs/12.input", "r");
  if (!f) {
    err(EXIT_FAILURE, "error reading input file");
  }
  char linebuf[BUFSIZ] = {0};
  while (fgets(linebuf, BUFSIZ, f) != NULL) {
    struct instruction* ins = &instructions[instructions_n++];
    char* s = linebuf;

    // parse single character
    ins->action = *s++;

    // parse digit
    ins->value = 0;
    while (*s >= '0' && *s <= '9') {
      ins->value = (ins->value * 10) + (*s - '0');
      s++;
    }
  }
  fclose(f);

  struct ship ship = {
      .pos = {0, 0},
      .waypoint = {10, 1},
  };
  for (int32_t i = 0; i < instructions_n; i++) {
    struct instruction ins = instructions[i];
#ifdef STEP
    printf("ship <%d, %d> \twaypoint <%d, %d>.\n", ship.pos.x, ship.pos.y,
           ship.waypoint.x, ship.waypoint.y);
    printf("%c%d\n", ins.action, ins.value);
    getc(stdin);
#endif
    switch (ins.action) {
      case 'F':
        ship.pos.x += ship.waypoint.x * ins.value;
        ship.pos.y += ship.waypoint.y * ins.value;
        break;
      case 'N':
      case 'E':
      case 'S':
      case 'W':
        move_waypoint(&ship, ins.action, ins.value);
        break;
      case 'L':
        rotate_waypoint(&ship, ins.value);
        break;
      case 'R':
        rotate_waypoint(&ship, -ins.value);
        break;
    }
  }

  int32_t manhattan_distance = abs(ship.pos.x) + abs(ship.pos.y);
  printf("%d\n", manhattan_distance);
  assert(manhattan_distance == 46530);

  free(instructions);
  return 0;
}
