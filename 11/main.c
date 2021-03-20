#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>

enum position {
    POS_FLOOR,
    POS_EMPTY_SEAT,
    POS_OCCUPIED_SEAT,
};

struct grid {
    int w;
    int h;
    enum position *values;
    char stable;
};

int count_occupied_adjacent_seats(struct grid *grid, int pos_y, int pos_x) {
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

void print_grid(struct grid *grid) {
    for (int y=0; y < grid->h; y++) {
        for (int x=0; x < grid->w; x++) {
            switch (*(grid->values + (y*grid->w) + x)) {
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

void transmute_grid(struct grid *grid) {
    enum position *new_grid = malloc(grid->h * grid->w * sizeof(enum position));
    if (new_grid == NULL) {
        err(EXIT_FAILURE, "oom");
    }
    grid->stable = 1;
    for (int y=0; y < grid->h; y++) {
        for (int x=0; x < grid->w; x++) {
            enum position value = *(grid->values + (y * grid->w) + x);
            enum position *new_value = (new_grid + (grid->w * y) + x) ;
            switch (value) {
               case POS_EMPTY_SEAT:
                    if (count_occupied_adjacent_seats(grid, y, x) == 0) {
                        *new_value = POS_OCCUPIED_SEAT;
                        grid->stable = 0;
                    } else  {
                        *new_value = POS_EMPTY_SEAT;
                    }
                    break;

                case POS_OCCUPIED_SEAT:
                    if (count_occupied_adjacent_seats(grid, y, x) >= 4) {
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
    free(grid->values);
    grid->values = new_grid;
}

int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};

    struct grid grid = {
        .h = 0,
        .w = 0,
        .stable = 0,
    };
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        grid.h++;
        grid.w = grid.w ?: strlen(linebuf) - 1;
    }
    printf("W: %d\tH: %d\n", grid.w, grid.h);
    grid.values = malloc(grid.h * grid.w * sizeof(enum position));
    fseek(f, 0, SEEK_SET);
    int y = 0;
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        char *s = linebuf;
        for (int x=0; *s != '\n' && *s != '\0'; s++, x++) {
            *(grid.values + y*grid.w + x)= *s == '.' ? POS_FLOOR : POS_EMPTY_SEAT;
        }
        y++;
    }
    fclose(f);

    // print grid in intitial position
    print_grid(&grid);
    while (grid.stable == 0) {
        transmute_grid(&grid);
    }
    printf("\n\n");
    print_grid(&grid);

    // count occupied seats
    int count = 0;
    for (int i=0; i < grid.h * grid.w; i++) {
        if (grid.values[i] == POS_OCCUPIED_SEAT) {
            count++;
        }
    }
    printf("%d occupied seats\n", count);
}
