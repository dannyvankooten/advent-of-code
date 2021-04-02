#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>

enum position {
    POS_FLOOR = -1,
    POS_EMPTY_SEAT = 0,
    POS_OCCUPIED_SEAT = 1,
};

struct grid {
    int w;
    int h;
    enum position *values;
    char stable;
};

typedef struct grid grid_t;

int count_occupied_adjacent_seats(grid_t *grid, int pos_y, int pos_x) {
    int y_start = pos_y - 1;
    y_start = y_start < 0 ? 0 : y_start;
    int y_end = pos_y + 1;
    y_end = y_end > grid->h - 1 ? grid->h-1 : y_end;
    int x_start = pos_x - 1 < 0 ? 0 : pos_x - 1;
    int x_end = pos_x + 1 > grid->w - 1 ? grid->w - 1 : pos_x + 1;

    int count = 0;
    for (int y = y_start; y <= y_end; y++) {
        for (int x = x_start; x <= x_end; x++) {
            if (y == pos_y && x == pos_x) {
                continue;
            }
            enum position v = *(grid->values + (y*grid->w) + x);
            if (v == POS_OCCUPIED_SEAT) {
                count++;
            }
        }
    }
    return count;
}

#define get_grid_value_by_coords(grid, x, y) *(grid->values + (y * grid->w) + x)

int count_occupied_seats_in_los(struct grid *grid, int pos_y, int pos_x) {
    int count = 0;
    enum position value;
    static const int directions[8][2] = {
        { 0, 1}, // right
        { 0, -1}, // left
        { 1, 0},  // down
        { -1, 0}, // up
        { 1, 1}, // down-right
        { -1, 1}, // up-right,
        { 1, -1}, // down-left
        { -1, -1}, // up-left
    };
    for (int d=0; d < 8; d++) {
        int dy = directions[d][0];
        int dx = directions[d][1];
        for (int y = pos_y + dy, x = pos_x + dx; y >= 0 && x >= 0 && x < grid->w && y < grid->h; x += dx, y += dy) {
            value = get_grid_value_by_coords(grid, x, y);
            if (value != POS_FLOOR) {
                count += value;
                break;
            }
        }
    }
    
    return count;
}

// static void print_grid(struct grid *grid) {
//     for (int y=0; y < grid->h; y++) {
//         for (int x=0; x < grid->w; x++) {
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

static void transmute_grid(struct grid *grid) {
    enum position new_grid[grid->h * grid->w];
    
    grid->stable = 1;
    for (int y=0; y < grid->h; y++) {
        for (int x=0; x < grid->w; x++) {
            enum position value = grid->values[(y * grid->w) + x];
            enum position *new_value = &new_grid[(grid->w * y) + x];
            switch (value) {
               case POS_EMPTY_SEAT:
                    if (count_occupied_seats_in_los(grid, y, x) == 0) {
                        *new_value = POS_OCCUPIED_SEAT;
                        grid->stable = 0;
                    } else  {
                        *new_value = POS_EMPTY_SEAT;
                    }
                    break;

                case POS_OCCUPIED_SEAT:
                    if (count_occupied_seats_in_los(grid, y, x) >= 5) {
                        *new_value = POS_EMPTY_SEAT;
                        grid->stable = 0;
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
    memcpy(grid->values, new_grid, grid->h * grid->w * sizeof(enum position));
}

int day11() {
    FILE *f = fopen("11.input", "r");
    if (!f) {
        err(EXIT_FAILURE, "error reading input file");
    }
    char linebuf[BUFSIZ] = {0};

    struct grid grid = {
        .h = 0,
        .w = 0,
        .stable = 0,
    };
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        grid.h++;
        if (grid.w == 0) {
            grid.w = (int) strlen(linebuf) - 1;
        }
    }
    grid.values = malloc(grid.h * grid.w * sizeof(enum position));
    if (!grid.values) {
        err(EXIT_FAILURE, "error allocating memory for grid values");
    }
    fseek(f, 0, SEEK_SET);
    int y = 0;
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        char *s = linebuf;
        for (int x=0; *s != '\n' && *s != '\0'; s++, x++) {
            *(grid.values + y*grid.w + x)= (*s == '.') ? POS_FLOOR : POS_EMPTY_SEAT;
        }
        y++;
    }
    fclose(f);

    while (!grid.stable) {
        transmute_grid(&grid);
    }

    // count occupied seats
    int count = 0;
    for (int i=0; i < grid.h * grid.w; i++) {
        if (grid.values[i] == POS_OCCUPIED_SEAT) {
            count++;
        }
    }
    printf("%d\n", count);
    
    free(grid.values);
    return 0;
}

