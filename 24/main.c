#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

typedef struct tile tile_t;
struct tile {
    enum {
        WHITE,
        BLACK,
    } color;
};

typedef enum direction direction_t;
enum direction {
    NE,
    NW,
    E,
    W,
    SE,
    SW
};

tile_t *grid;
int32_t black_tile_count = 0;

void
flip_tile(tile_t *t) {
    switch (t->color) {
        case WHITE:
            black_tile_count++;
            t->color = BLACK;
            break;

        case BLACK:
            black_tile_count--;
            t->color = WHITE;
            break;
    }
}

void 
print_direction(direction_t d) {
    static const char *names[] = {"NE", "NW", "E", "W", "SE", "SW"};
    printf("%s", names[d]);
}

size_t 
parse_line(direction_t *directions, char *line) {
    char *s = line;
    size_t ndirections = 0;
    while (*s != '\n' && *s != '\0') {
        switch (*s++) {
            case 'e':
                directions[ndirections++] = E;
            break;

            case 'w':
                directions[ndirections++] = W;
            break;

            case 'n':
                switch(*s++) {
                    case 'e':
                        directions[ndirections++] = NE;
                    break;

                    case 'w':
                        directions[ndirections++] = NW;
                    break;
                }
            break;

            case 's':
                switch(*s++) {
                    case 'e':
                        directions[ndirections++] = SE;
                    break;

                    case 'w':
                        directions[ndirections++] = SW;
                    break;
                }
            break;
        }
    }

    return ndirections;
}


int main() {
    const int32_t GRIDSIZE = 64;
    tile_t *grid = (tile_t *) malloc(sizeof(tile_t) * GRIDSIZE * GRIDSIZE);
    if (!grid) return 1;
    for (int i=0; i < GRIDSIZE*GRIDSIZE; i++) {
        grid[i].color = WHITE;
    }
    int32_t x, y;

    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    direction_t *directions = (direction_t *) malloc(sizeof(direction_t) * 50);
    if (!directions) return 1;

    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        size_t ndirections = parse_line(directions, linebuf);
        // go back to start tile 
        x = 32;
        y = 32;

        for (size_t i=0; i < ndirections; i++) {
            switch (directions[i]) {
                case E:
                    x += 1;
                break;

                case W:
                    x -= 1;
                break;

                case NE:
                    if (y % 2 != 0) {
                        x++;
                    }
                    y -= 1;
                break;

                case NW:
                    if (y % 2 == 0) {
                        x--;
                    }
                    y -= 1;
                break;

                case SE:
                    if (y % 2 != 0) {
                        x++;
                    }
                    y += 1;
                break;

                case SW:
                    if (y % 2 == 0) {
                        x--;
                    }
                    y += 1;
                break;
            }
        }
        flip_tile(&grid[(y * GRIDSIZE) + x]);
        
    }
    fclose(f);

    // cound black tiles
    printf("%d\n", black_tile_count);

    free(directions);
    free(grid);
}
