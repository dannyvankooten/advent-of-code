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

    tile_t *n;
    tile_t *e;
    tile_t *s;
    tile_t *w;
};

#define W 10
#define H 10

int 
parse_tiles_from_input(tile_t *tiles) {
    int ntiles = 0;
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    while (fgets(linebuf, BUFSIZ, f) != NULL) {

        // first line contains the tile ID: Tile 2311:
        tile_t t;
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
                    grid[y * W + (9-x)] = tile->grid[y * W + x];
                break;

                case 'y':
                    grid[(9-y) * W + x] = tile->grid[y * W + x];
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


int main() {
    tile_t *tiles = malloc(200 * sizeof(tile_t));
    if (!tiles) {
        err(EXIT_FAILURE, "could not allocate memory for tiles");
    }

    int ntiles = parse_tiles_from_input(tiles);
    print_tiles(tiles, ntiles);

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
    int img_size = (int) sqrt((double) ntiles);
    printf("Image size is %d x %d\n", img_size, img_size);
    int *image = calloc(img_size * img_size, sizeof(int));
    for (int y = 0; y < img_size; y++) {
        for (int x = 0; x < img_size; x++) {
            printf("%d\t", image[y * img_size + x]);
        }
        printf("\n");
    }

    // start by finding the middle tile
    // which is a tile that should have 4 possible neighbors
    printf("%d\n", 3/2);
    long long product = 1;
    for (int i=0; i < ntiles; i++) {
        tile_t *t1 = &tiles[i];

        printf("Finding neighbors for %d: \n", t1->id);
        int neighbors = 0;

        for (int j=0; j < ntiles; j++) {
            // skip self
            if (i == j) { continue; }
            tile_t *t2 = &tiles[j];

            for (int r=0; r < 4; r++) {
                for (int f=0; f < 2; f++) {
                    if (cmp_tile_edges(t1, t2, 'N') == 0) {
                        printf("Found N: %d\n", t2->id);
                        t1->n = t2;
                        neighbors++;
                    } else if (cmp_tile_edges(t1, t2, 'E') == 0) {
                        printf("Found E: %d\n", t2->id);
                        t1->e = t2;
                        neighbors++;
                    } else if (cmp_tile_edges(t1, t2, 'S') == 0) {
                        printf("Found S: %d\n", t2->id);
                        t1->s = t2;
                        neighbors++;
                    } else if (cmp_tile_edges(t1, t2, 'W') == 0) {
                        printf("Found W: %d\n", t2->id);
                        t1->w = t2;
                        neighbors++;
                    }     

                    flip_tile(t2, f > 0 ? 'x' : 'y');
                }

                rotate_tile(t2);   
            }    
        }

        if (neighbors == 2) {
            product *= t1->id;
        }

        
    }

    printf("Product: %lld\n", product);



    free(image);
    free(tiles);
}
