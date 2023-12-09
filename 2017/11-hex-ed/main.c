#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../adventofcode.h"

#define PUZZLE_NAME "Day 11: Hex Ed"

typedef struct {
    int q; // col
    int r; // row;
} hex_t;

hex_t directions[6] = {
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, 0},
    {-1, 1},
    {0, 1},
};

hex_t axial_add(hex_t a, hex_t b) {
    return (hex_t) {
        .q = a.q + b.q,
        .r = a.r + b.r,
    };
};

hex_t axial_step(hex_t h, unsigned char dir) {
    return axial_add(h, directions[dir]);
}

int axial_distance(hex_t a, hex_t b) {
    return (abs(a.q - b.q)
            + abs(a.q + a.r - b.q - b.r)
            + abs(a.r - b.r)) / 2;
}

int main() {
    clock_t t = timer_start();
    char input[1024 * 64];
    read_input_file(input, "input.txt");

    hex_t pos = (hex_t) {
        .r = 0,
        .q = 0
    };
    hex_t start = pos;
    int max_distance = -1;
    char *s = input;
    char dir[8];
    while (*s != '\0') {
        s = parse_ident(dir, s);
        int index;
        if (strncmp(dir, "ne", 2) == 0) {
            index= 1;
        } else if (strncmp(dir, "se", 2) == 0) {
            index = 0;
        } else if (strncmp(dir, "sw", 2) == 0) {
            index = 4;
        } else if (strncmp(dir, "nw", 2) == 0) {
            index = 3;
        } else if (strncmp(dir, "n", 1) == 0) {
            index = 2;
        } else if (strncmp(dir, "s", 1) == 0) {
            index = 5;
        } else {
            fprintf(stderr, "invalid direction: %s\n", dir);
            exit(EXIT_FAILURE);
        }


        pos = axial_step(pos, index);
        int d = axial_distance(pos, start);
        if (d > max_distance) {
            max_distance = d;
        }

        if (*s == ',' || *s == '\n') s++;
    }

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", axial_distance(pos, start));
    printf("Part 2: %d\n", max_distance);
    printf("Time: %.2fms\n", timer_stop(t));
    return EXIT_SUCCESS;
}
