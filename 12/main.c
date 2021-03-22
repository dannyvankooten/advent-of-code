#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <math.h>

struct instruction {
    char action;
    int value;
};

struct position {
    int x;
    int y;
};

struct ship {
    struct position pos;
    struct position waypoint;
};

void move_waypoint(struct ship *ship, char dir, int value) {
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

void rotate_waypoint(struct ship *ship, int value) {
    double v = (double) value * M_PI / 180.0;
    int x1 = round(ship->waypoint.x * cos(v) - ship->waypoint.y * sin(v));
    int y1 = round(ship->waypoint.x * sin(v) + ship->waypoint.y * cos(v));
    ship->waypoint.x = x1;
    ship->waypoint.y = y1;
}

int main() {
    int instructions_n = 0;
    struct instruction *instructions = malloc(800 * sizeof(struct instruction));
    if (!instructions) err(EXIT_FAILURE, "oom");

    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    char *s;
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        struct instruction ins;
        s = linebuf;
        ins.action = *s++;
        ins.value = (int) strtol(s, &s+2, 10);
        instructions[instructions_n++] = ins;        
    }
    fclose(f);

    struct ship ship = {
        .pos = {0, 0},
        .waypoint = {10, 1},
    };
    for (int i=0; i<instructions_n; i++) {
        struct instruction ins = instructions[i];
         #ifdef STEP
        printf("ship <%d, %d> \twaypoint <%d, %d>.\n", ship.pos.x, ship.pos.y, ship.waypoint.x, ship.waypoint.y);
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

    printf("Final position: (%d, %d)\n", ship.pos.x, ship.pos.y);
    printf("Manhattan distance: %d\n", abs(ship.pos.x) + abs(ship.pos.y));

    free(instructions);
}
