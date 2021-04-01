#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct tile tile_t;

struct tile {
    int32_t id;
    char grid[100];

    int32_t x;
    int32_t y;
};

#define W 10
#define H 10

size_t 
parse_tiles_from_input(tile_t *tiles, char *file) {
    size_t ntiles = 0;
    FILE *f = fopen(file, "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    while (fgets(linebuf, BUFSIZ, f) != NULL) {

        // first line contains the tile ID: Tile 2311:
        tile_t t = {
            .y = -1,
            .x = -1,
        };
        sscanf(linebuf, "Tile %d:", &t.id);

        size_t y = 0;
        while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
            char *s = linebuf;
            for (size_t x=0; *s != '\n' && *s != '\0'; s++, x++) {
                t.grid[y * W + x] = *s;
            }

            y++;
        }

        tiles[ntiles++] = t;


    }
    fclose(f);

    return ntiles;
}

void print_tile(tile_t *tile) {
    printf("Tile %d:\n", tile->id);
    for (size_t y=0; y < H; y++) {
        for (size_t x=0; x < W; x++) {
            printf("%c", tile->grid[y * W + x]);
        }
        printf("\n");
    }
}

void print_tiles(tile_t *tiles, size_t ntiles) {
    for (size_t i=0; i < ntiles; i++) {
        print_tile(&tiles[i]);
    }
}

// https://en.wikipedia.org/wiki/Cartesian_coordinate_system#Rotation
void rotate(char *grid, size_t size) {
    char new_grid[size * size];
    for (size_t y=0, x2=size-1; y < size; y++, x2--) {
        for (size_t x=0, y2=0; x < size; x++, y2++) {
            new_grid[y2 * size + x2] = grid[y * size + x];
        }
    }

    for (size_t i=0; i < size * size; i++) {
        grid[i] = new_grid[i];
    } 
}

void flip(char *grid, size_t size, char ax) {
    // 0 1 2 3 4 5 6 7 8 9 
    // 9 8 7 6 5 4 3 2 1 0
    char new_grid[size*size];
    for (size_t y=0; y < size; y++) {
        for (size_t x=0; x < size; x++) {
            switch (ax) {
                case 'x':
                    new_grid[y * size + (size-1-x)] = grid[y * size + x];
                break;

                case 'y':
                    new_grid[(size-1-y) * size + x] = grid[y * size + x];
                break;
            }
        }
    }

    for (size_t i=0; i < size * size; i++) {
        grid[i] = new_grid[i];
    }
}

bool
tiles_edges_match(tile_t *t1, tile_t *t2, char edge) {
    size_t x1, x2, y1, y2;

    // edge can be one of 'N', 'E', 'S', 'W'
    switch (edge) {
        case 'N':
        case 'S':
            y1 = edge == 'N' ? 0 : H-1;
            y2 = edge == 'N' ? H-1 : 0;
            for (x1=0; x1 < W; x1++) {
                if (t1->grid[y1 * W + x1] != t2->grid[y2 * W + x1]) {
                    return false;
                }
            }
            return true;
        break;

        case 'E':
        case 'W':
            x1 = edge == 'E' ? W-1 : 0;
            x2 = edge == 'E' ? 0 : W - 1;
            for (y1=0; y1 < H; y1++) {
                if (t1->grid[y1 * W + x1] != t2->grid[y1 * W + x2]) {
                    return false;
                }
            }
            return true;
        break;  

        default:
            err(EXIT_FAILURE, "invalid edge argument: %c", edge);
        break;  
    }
}

void 
remove_image_borders(char *image, tile_t **tiles, size_t image_size)  {
    size_t img_y = 0;
    size_t img_x = 0;
    size_t img_width = (W-2) * image_size;
    for (size_t image_row=0; image_row < image_size; image_row++) {
        for (size_t y=1; y < H-1; y++) {
            for (size_t ti=0; ti < image_size; ti++) {
                tile_t *t = tiles[image_row * image_size + ti];
                for (size_t x=1; x < W-1; x++) {
                    image[img_y * img_width + img_x++] = t->grid[y * W + x];
                }
            }
        }
    }
}


void 
print_image(tile_t **image, size_t image_size) {
    for (size_t image_y = 0; image_y < image_size; image_y++) {
        for (size_t y=0; y < H; y++) {
            for (size_t image_x = 0; image_x < image_size; image_x++) {
                tile_t *t = image[image_y * image_size + image_x];
                for (size_t x=0; x < W; x++) {
                    printf("%c", t != NULL ? t->grid[y * W + x] : '?');
                }
            }
          
            printf("\n");
        }
    }
}

void 
print_image_ids(tile_t **image, size_t size) {
    for (size_t y = 0; y < size; y++) {
        for (size_t x = 0; x < size; x++) {
            tile_t *t = image[y * size + x];
            printf("%d\t", t != NULL ? t->id : 0);
        }
        printf("\n");
    }
}


void
shift_image(tile_t **image, size_t size, int8_t shift_y, int8_t shift_x) {
    tile_t *new_image[size*size];
    for (size_t i=0; i < size*size; i++) new_image[i] = NULL;
    tile_t *t;

    for (size_t y=0; y < size-shift_y; y++) {
        for (size_t x=0; x < size-shift_x; x++) {
            int8_t y2 = y + shift_y;
            int8_t x2 = x + shift_x;

            t = image[y * size + x];
            if (t) {
                t->x = x2;
                t->y = y2;
                new_image[y2 * size + x2] = t;
            }
        }
    }

    for (size_t i=0; i < size*size; i++) {
        image[i] = new_image[i];
    }
}

size_t
count_sea_monster_in_image(const char *image, size_t image_size, const char *monster) 
{
    size_t count = 0;
    for (size_t y=0; y < image_size; y++) {
        for (size_t x=0; x < image_size; x++) {
            const char *i = &image[y * image_size + x];
            const char *m = monster;
            size_t y_search = y;
            size_t x_search = x;

            while (*i == *m || *m == ' ') {
                i++;
                m++;

                if (*m == '\n') {
                    y_search++;
                    x_search = x;
                    i = &image[y_search * image_size + x_search];
                    m++;
                }

                if (*m == '\0') {
                    count++;
                }
            }
        }
    }

    return count;
}


int main() {
    tile_t *tiles = malloc(200 * sizeof(tile_t));
    if (!tiles) {
        err(EXIT_FAILURE, "could not allocate memory for tiles");
    }

    size_t ntiles = parse_tiles_from_input(tiles, "input.txt");
    // print_tiles(tiles, ntiles);

    // init empty image
    size_t image_size = (size_t) sqrt((double) ntiles);
    tile_t **image = calloc(image_size * image_size * 2, sizeof(tile_t *));

    // For each tile, find NESW neighbors and place in image grid
    image[0] = &tiles[0];
    tiles[0].y = 0;
    tiles[0].x = 0;

    STARTOVER: ;
    for (size_t i=0; i < ntiles; i++) {
        // only work with tiles already in image
        tile_t *t1 = &tiles[i];
        if (t1->y < 0) {
            continue;
        }

        for (size_t j=0; j < ntiles; j++) {
            // skip self
            if (i == j) continue;

            // skip tiles already in image
            tile_t *t2 = &tiles[j];
            if (t2->x >= 0) {
                continue;
            }

            for (int8_t r = 0; r < 2; r++) {
                for (int8_t f=0; f < 2; f++) {
                    if (tiles_edges_match(t1, t2, 'E')) {
                        if (t1->x == image_size-1) {
                            shift_image(image, image_size, 0, -1);
                        }
                        t2->y = t1->y;
                        t2->x = t1->x + 1;
                        image[t2->y * image_size + t2->x] = t2;
                        goto STARTOVER;
                    } else if (tiles_edges_match(t1, t2, 'S')) {
                        if (t1->y == image_size - 1) {
                            shift_image(image, image_size, -1, 0);
                        }
                        t2->y = t1->y + 1;
                        t2->x = t1->x;
                       
                        image[t2->y * image_size + t2->x] = t2;
                        goto STARTOVER;
                    } else if (tiles_edges_match(t1, t2, 'N')) {
                        // if t1 was at northern edge, shift it down
                        if (t1->y == 0) {
                            shift_image(image, image_size, 1, 0);
                        }
                        t2->y = t1->y - 1;
                        t2->x = t1->x;
                        image[t2->y * image_size + t2->x] = t2;
                        goto STARTOVER;
                    }  else if (tiles_edges_match(t1, t2, 'W')) {
                        // if t1 was at western edge, shift it down
                        if (t1->x == 0) {
                            shift_image(image, image_size, 0, 1);
                        }
                        t2->y = t1->y;
                        t2->x = t1->x - 1;
                        image[t2->y * image_size + t2->x] = t2;
                        goto STARTOVER;
                    }

                    flip(t2->grid, W, f > 0 ? 'x' : 'y');
                }

                rotate(t2->grid, W);
            }
        }
    }

    // print_image_ids(image, image_size);
    // print_image(image, image_size);


    // generate our final image, plain char array
    size_t final_image_size = (W-2) * image_size;
    char *final_image = malloc((W-2) * image_size * (H-2) * image_size);
    remove_image_borders(final_image, image, image_size);

    const char *sea_monster = "                  # \n"
        "#    ##    ##    ###\n"
        " #  #  #  #  #  #   ";
    int64_t sea_monster_count = 0;
    for (size_t r=0; r < 3; r++) {
        for(size_t f=0; f < 2; f++) {
            
            sea_monster_count = count_sea_monster_in_image(final_image, final_image_size, sea_monster);
            if (sea_monster_count > 0) {
                goto DETERMINE_WATER_ROUGHNESS;
            }
            
            flip(final_image, final_image_size, 'x');
            
        }
        rotate(final_image, final_image_size);
    }

    DETERMINE_WATER_ROUGHNESS: ;
    
    // count # in monster
    size_t c1 = 0;
    const char *m = sea_monster;
    while (*m != '\0') {
        if (*m == '#') {
            c1++;
        }

        m++;
    }

    // count # in image
    size_t c2 = 0;
    for (size_t i=0; i < final_image_size * final_image_size; i++) {
        if (final_image[i] == '#') {
            c2++;
        }
    }

    printf("%ld\n", c2 - (c1 * sea_monster_count));

    free(final_image);
    free(image);
    free(tiles);
}
