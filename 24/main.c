#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

typedef enum {
    WHITE = 0,
    BLACK = 1,
} color_t;

typedef enum direction direction_t;
enum direction {
    NE,
    NW,
    E,
    W,
    SE,
    SW
};

const int32_t GRIDSIZE = 256;
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
    
    // TODO
    return count;
}

void
apply_rules(color_t *grid) {
    color_t new_grid[GRIDSIZE * GRIDSIZE];

    for (int32_t y=0; y < GRIDSIZE; y++) {
        for (int32_t x=0; x < GRIDSIZE; x++) {
            int32_t black_neighbors = count_adjacent_black_tiles(grid, x, y); 
            // printf("(%d, %d): %d adjacent black tiles\n", x, y, black_neighbors);
            new_grid[y * GRIDSIZE + x] = grid[y * GRIDSIZE + x];
            switch (grid[y * GRIDSIZE + x]) {
                case WHITE:
                    if (black_neighbors == 2) {
                        new_grid[y * GRIDSIZE + x] = BLACK;
                        black_tile_count++;
                    }
                break;

                case BLACK:
                    if (black_neighbors == 0 || black_neighbors > 2) {
                        new_grid[y * GRIDSIZE + x] = WHITE;
                        black_tile_count--;
                    }
                break;
            }
        }
    }

    for (int32_t i = 0; i < GRIDSIZE*GRIDSIZE; i++) {
        grid[i] = new_grid[i];
    }
}


int main() {
    color_t *grid = (color_t *) malloc(sizeof(color_t) * GRIDSIZE * GRIDSIZE);
    if (!grid) return 1;
    for (int32_t i=0; i < GRIDSIZE*GRIDSIZE; i++) {
        grid[i] = WHITE;
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
        x = GRIDSIZE / 2;
        y = GRIDSIZE / 2;

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

    for (int32_t i=0; i < 100; i++) {
        apply_rules(grid);
    }
    printf("%d\n", black_tile_count);

    free(directions);
    free(grid);
}
