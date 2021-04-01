#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

typedef enum {
    WHITE = 0,
    BLACK = 1,
} color_t;

typedef enum {
    NE,
    NW,
    E,
    W,
    SE,
    SW
} direction_t;

const int32_t GRIDSIZE = 128;
const int32_t GRIDSIZE_SQ = GRIDSIZE * GRIDSIZE;
int32_t black_tile_count = 0;

void
flip_tile(color_t *color) {
    switch (*color) {
        case WHITE:
            black_tile_count++;
            *color = BLACK;
            break;

        case BLACK:
            black_tile_count--;
            *color = WHITE;
            break;
    }
}

void 
print_direction(direction_t d) {
    static const char *names[] = {"NE", "NW", "E", "W", "SE", "SW"};
    printf("%s", names[d]);
}

int32_t 
parse_line(direction_t *directions, char *line) {
    char *s = line;
    int32_t ndirections = 0;
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

int32_t 
count_adjacent_black_tiles(color_t *grid, int32_t x, int32_t y) {
    int32_t count = 0;

    // west
    if (x > 0) {
         count += grid[(y * GRIDSIZE) + x - 1];
    }

    // east
    if (x < GRIDSIZE-1) {
         count += grid[(y * GRIDSIZE) + x + 1];
    }
     

    if (y > 0) {
        if (y % 2 == 0) {
            // north-east
            count += grid[(y - 1) * GRIDSIZE + x];

            // north west
            if (x > 0) {
                count += grid[(y - 1) * GRIDSIZE + x - 1];
            }
        } else {
            // north-east
            if (x < GRIDSIZE-1) {
                count += grid[(y - 1) * GRIDSIZE + x + 1];
            }

            // north west
            count += grid[(y - 1) * GRIDSIZE + x];
        }      
    }

    if (y < GRIDSIZE-1) {
        if (y % 2 == 0) {
            // south-east
            count += grid[(y + 1) * GRIDSIZE + x];

            // south west
            if (x > 0) {
                count += grid[(y + 1) * GRIDSIZE + x - 1];
            }
        } else {
            // south-east
            if (x < GRIDSIZE-1) {
                count += grid[(y + 1) * GRIDSIZE + x + 1];
            }

            // south west
            count += grid[(y + 1) * GRIDSIZE + x];
        }        
    }
    
    return count;
}

void
apply_rules(color_t *grid) {
    color_t new_grid[GRIDSIZE_SQ];

    for (int32_t y=0; y < GRIDSIZE; y++) {
        for (int32_t x=0; x < GRIDSIZE; x++) {
            int32_t black_neighbors = count_adjacent_black_tiles(grid, x, y); 
            color_t *color = &grid[y * GRIDSIZE + x];
            color_t *new_color = &new_grid[y * GRIDSIZE + x];
            switch (*color) {
                case WHITE:
                    if (black_neighbors == 2) {
                        *new_color = BLACK;
                        black_tile_count++;
                    } else {
                        *new_color = WHITE;
                    }
                break;

                case BLACK:
                    if (black_neighbors == 0 || black_neighbors > 2) {
                        *new_color = WHITE;
                        black_tile_count--;
                    } else {
                        *new_color = BLACK;
                    }
                break;
            }
        }
    }

    for (int32_t i=0; i < GRIDSIZE_SQ; i++) {
        grid[i] = new_grid[i];
    }
}


int main() {
    color_t *grid = (color_t *) calloc(GRIDSIZE_SQ, sizeof(color_t));
    if (!grid) {
        return 1;
    }

    FILE *f = fopen("input.txt", "r");
    if (!f) {
        err(EXIT_FAILURE, "error reading input file");
    }
    char linebuf[BUFSIZ] = {0};
    direction_t *directions = (direction_t *) malloc(sizeof(direction_t) * 50);
    if (!directions) {
        return 1;
    }

    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        int32_t ndirections = parse_line(directions, linebuf);
        
        // go back to start tile 
        int32_t x = GRIDSIZE / 2;
        int32_t y = x;

        for (int32_t i=0; i < ndirections; i++) {
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
    assert(black_tile_count == 244);

    for (int32_t i=0; i < 100; i++) {
        apply_rules(grid);
    }
    printf("%d\n", black_tile_count);
    assert(black_tile_count == 3665);

    free(directions);
    free(grid);
}
