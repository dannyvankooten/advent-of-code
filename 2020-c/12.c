#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846 /* pi */

struct instruction {
  int value;
  char action;
};

struct position {
  int x;
  int y;
};

struct ship {
  struct position pos;
  struct position waypoint;
};

static void
move_waypoint(struct ship* ship, const char dir, const int value) {
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

static void err(int status, char *message) {
  fputs(message, stderr);
  exit(status);
}

static void
rotate_waypoint(struct ship* ship, const int value) {
  const double v = (double)value * M_PI / 180.0;
  const int x1 = round(ship->waypoint.x * cos(v) - ship->waypoint.y * sin(v));
  const int y1 = round(ship->waypoint.x * sin(v) + ship->waypoint.y * cos(v));
  ship->waypoint.x = x1;
  ship->waypoint.y = y1;
}

int day12() {
  int instructions_n = 0;
  struct instruction instructions[1024];
  FILE* f = fopen("inputs/12.txt", "r");
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
  for (int i = 0; i < instructions_n; i++) {
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

  int manhattan_distance = abs(ship.pos.x) + abs(ship.pos.y);
  printf("%d\n", manhattan_distance);
  assert(manhattan_distance == 46530);
  return 0;
}
