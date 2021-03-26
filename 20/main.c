#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include <math.h>

typedef struct tile tile_t;

struct tile {
    int id;
    char grid[100];

    int x;
    int y;
};

#define W 10
#define H 10

int 
parse_tiles_from_input(tile_t *tiles) {
    int ntiles = 0;
    FILE *f = fopen("test_input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    while (fgets(linebuf, BUFSIZ, f) != NULL) {

        // first line contains the tile ID: Tile 2311:
        tile_t t = {
            .y = -1,
            .x = -1,
        };
        sscanf(linebuf, "Tile %d:", &t.id);

        int y = 0;
        while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
            char *s = linebuf;
            for (int x=0; *s != '\n' && *s != '\0'; s++, x++) {
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
    for (int y=0; y < H; y++) {
        for (int x=0; x < W; x++) {
            printf("%c", tile->grid[y * W + x]);
        }
        printf("\n");
    }
}

void print_tiles(tile_t *tiles, int ntiles) {
    for (int i=0; i <ntiles; i++) {
        print_tile(&tiles[i]);
    }
}

// https://en.wikipedia.org/wiki/Cartesian_coordinate_system#Rotation
void 
rotate_tile(tile_t *tile) {
    char grid[100];
    for (int y=0, x2=W-1; y < H; y++, x2--) {
        for (int x=0, y2=0; x < W; x++, y2++) {
            grid[y2 * W + x2] = tile->grid[y * W + x];
        }
    }

    for (int i=0; i < W * H; i++) tile->grid[i] = grid[i];
}

void 
flip_tile(tile_t *tile, char ax) {
    // 0 1 2 3 4 5 6 7 8 9 
    // 9 8 7 6 5 4 3 2 1 0
    char grid[100];
    for (int y=0; y < H; y++) {
        for (int x=0; x < W; x++) {
            switch (ax) {
                case 'x':
                    grid[y * W + (W-1-x)] = tile->grid[y * W + x];
                break;

                case 'y':
                    grid[(H-1-y) * W + x] = tile->grid[y * W + x];
                break;
            }
        }
    }

    for (int i=0; i < W * H; i++) tile->grid[i] = grid[i];
}

int 
cmp_tiles(tile_t *t1, tile_t *t2) {
    for (int i=0; i < W * H; i++) {
        if (t1->grid[i] != t2->grid[i]) {
            return 1;
        }
    }

    return 0; // grids match
}

int
cmp_tile_edges(tile_t *t1, tile_t *t2, char edge) {
    int x, y;

    // edge can be one of 'N', 'E', 'S', 'W'
    switch (edge) {
        case 'N':
        case 'S':
            y = edge == 'N' ? 0 : H-1;
            for (x=0; x < W; x++) {
                if (t1->grid[y * W + x] != t2->grid[y * W + x]) {
                    return 1;
                }
            }
            return 0;
        break;

        case 'E':
        case 'W':
            x = edge == 'E' ? W-1 : 0;
            for (y=0; y < H; y++) {
                if (t1->grid[y * W + x] != t2->grid[y * W + x]) {
                    return 1;
                }
            }
            return 0;
        break;  

        default:
            err(EXIT_FAILURE, "invalid edge argument: %c", edge);
        break;  
    }
}

void 
print_image(tile_t **image, int size) {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            tile_t *t = image[y * size + x];
            printf("%d\t", t != NULL ? t->id : 0);
        }
        printf("\n");
    }
}

void
rotate_image(tile_t **image, int size) {
    tile_t *new_image[size*size];
    for (int i=0; i < size*size; i++) new_image[i] = NULL;
    tile_t *t;
    for (int y=0, x2=size-1; y < size; y++, x2--) {
        for (int x=0, y2=0; x < size; x++, y2++) {
            t = image[y * size + x];
            new_image[y2 * size + x2] = t;
            if (t) {
                t->x = x2;
                t->y = y2;
                rotate_tile(t);
            }
        }
    }

    for (int i=0; i < size*size; i++) image[i] = new_image[i];
}

void
flip_image(tile_t **image, int size, char ax) {
    tile_t *new_image[size*size];
    for (int i=0; i < size*size; i++) new_image[i] = NULL;
    tile_t *t;
    for (int y=0; y < size; y++) {
        for (int x=0; x < size; x++) {
            t = image[y * size + x];
            int y2 = ax == 'y' ? (size - 1 - y) : y;
            int x2 = ax == 'x' ? (size - 1 - x) : x;
            new_image[y2 * size + x2] = t;
            if (t) {
                t->x = x2;
                t->y = y2;
                flip_tile(t, ax);
            }
        }
    }

    for (int i=0; i < size*size; i++) image[i] = new_image[i];
}

void
shift_image(tile_t **image, int size, int shift_y, int shift_x) {
    tile_t *new_image[size*size];
    for (int i=0; i < size*size; i++) new_image[i] = NULL;
    tile_t *t;

    for (int y=0; y < size-shift_y; y++) {
        for (int x=0; x < size-shift_x; x++) {
            int y2 = y + shift_y;
            int x2 = x + shift_x;

            t = image[y * size + x];
            if (t) {
                t->x = x2;
                t->y = y2;
                new_image[y2 * size + x2] = t;
            }
        }
    }

    for (int i=0; i < size*size; i++) image[i] = new_image[i];
}

tile_t *
get_tile_by_id(tile_t *tiles, int ntiles, int id) {
    for (int i=0; i < ntiles; i++) {
        if (tiles[i].id == id) {
            return &tiles[i];
        }
    }

    return NULL;
}

int main() {
    tile_t *tiles = malloc(200 * sizeof(tile_t));
    if (!tiles) {
        err(EXIT_FAILURE, "could not allocate memory for tiles");
    }

    int ntiles = parse_tiles_from_input(tiles);
    // print_tiles(tiles, ntiles);

    #ifndef NDEBUG
    tile_t tmp;
    memcpy(&tmp, &tiles[0], sizeof(tile_t));
    flip_tile(&tiles[0], 'x');
    assert(cmp_tiles(&tiles[0], &tmp) != 0);
    flip_tile(&tiles[0], 'x');
    assert(cmp_tiles(&tiles[0], &tmp) == 0);
    rotate_tile(&tiles[0]);
    rotate_tile(&tiles[0]);
    assert(cmp_tiles(&tiles[0], &tmp) != 0);
    rotate_tile(&tiles[0]);
    rotate_tile(&tiles[0]);
    assert(cmp_tiles(&tiles[0], &tmp) == 0);
    #endif

    // init empty image
    int image_size = (int) sqrt((double) ntiles);
    tile_t **image = calloc(image_size * image_size * 2, sizeof(tile_t *));

    // For each tile, find NESW neighbors and place in image grid
    // Once placed, stop rotating the individual tiles
    // But allow rotation of the entire grid (image + tiles itself)
    image[0] = &tiles[0];
    tiles[0].y = 0;
    tiles[0].x = 0;
    print_image(image, image_size);


    STARTOVER: ;
    for (int i=0; i < ntiles; i++) {
        tile_t *t1 = &tiles[i];

        // only work with tiles already in image
        if (t1->y < 0) {
            continue;
        }

        printf("Finding neighbors for tile %d\n", t1->id);
        for (int j=0; j < ntiles; j++) {
            // skip self
            if (i == j) continue;

            // skip tiles already in image
            tile_t *t2 = &tiles[j];
            if (t2->x >= 0) {
                continue;
            }

            for (int r = 0; r < 3; r++) {
                for (int f=0; f < 2; f++) {
                    if (cmp_tile_edges(t1, t2, 'E') == 0) {
                        if (t1->x == image_size-1) {
                            shift_image(image, image_size, 0, -1);
                        }
                        t2->y = t1->y;
                        t2->x = t1->x + 1;
                        if (image[t2->y * image_size + t2->x] != NULL) {
                            if (t1->x == 0) {
                                shift_image(image, image_size, 0, 1);
                            }
                            t2->x = t1->x - 1;
                        }
                        image[t2->y * image_size + t2->x] = t2;
                        printf("Found E neighbor.\n");
                        goto STARTOVER;
                    } else if (cmp_tile_edges(t1, t2, 'S')) {
                        if (t1->y == image_size - 1) {
                            shift_image(image, image_size, -1, 0);
                        }
                        t2->y = t1->y + 1;
                        t2->x = t1->x;
                        if (image[t2->y * image_size + t2->x] != NULL) {
                            if (t1->y == 0) {
                                shift_image(image, image_size, 1, 0);
                            }
                            t2->y = t1->y - 1;                       
                        }
                        image[t2->y * image_size + t2->x] = t2;
                        printf("Found S neighbor.\n");
                        goto STARTOVER;
                    }
                  
                    flip_image(image, image_size, f > 0 ? 'x' : 'y');
                }
                
                rotate_image(image, image_size);     
            }
        }
    }
    print_image(image, image_size);


    free(image);
    free(tiles);
}
