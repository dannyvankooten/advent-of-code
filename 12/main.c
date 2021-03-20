#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

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
    char dir;
};

void move_ship(struct ship *ship, char dir, int value) {
    switch (dir) {
        case 'N':
            ship->pos.y -= value;
            break;
        case 'E':
            ship->pos.x += value;
            break;
        case 'S':
            ship->pos.y += value;
            break;
        case 'W':
            ship->pos.x -= value;
            break;
    }
}

void turn_ship(struct ship *ship, int value) {
    char directions[4] = {'N', 'E', 'S', 'W' };
    value /= 90;
    int dir = 0;
    switch(ship->dir) {
        case 'N':
            dir = 0;
            break;
        case 'E':
            dir = 1;
            break;
        case 'S': 
            dir = 2;
            break;
        case 'W':
            dir = 3;
            break;
    }

    dir += value;
    dir = dir < 0 ? dir + 4 : dir;
    dir = dir > 3 ? dir - 4 : dir;
    ship->dir = directions[dir];
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
        .dir = 'E'
    };
    for (int i=0; i<instructions_n; i++) {
        struct instruction ins = instructions[i];
        switch (ins.action) {
            case 'F':
                move_ship(&ship, ship.dir, ins.value);
                break;
            case 'N':
            case 'E':
            case 'S':
            case 'W':
                move_ship(&ship, ins.action, ins.value);
                break;
            case 'L':
                turn_ship(&ship, -ins.value);
                break;
            case 'R':
                turn_ship(&ship, ins.value);
                break;
        }
    }

    printf("Final position: (%d, %d)\n", ship.pos.x, ship.pos.y);
    printf("Manhattan distance: %d\n", abs(ship.pos.x) + abs(ship.pos.y));

    free(instructions);
}
