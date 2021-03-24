#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>

const char CH_ACTIVE = '#';
const char CH_INACTIVE = '.';

struct grid {
    int width;
    int height;
    int depth;
    int time;
    char *values;
};
typedef struct grid grid_t;

grid_t
read_input() {
    char linebuf[BUFSIZ] = {0};
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");

    // assume high enough values to fit our infinite grid
    grid_t g = {
        .width = 21,
        .height = 21,
        .depth = 21,
        .time = 21,
    };
    g.values = (char *) malloc(g.time * g.depth * g.width * g.height * sizeof(char));
    if (!g.values) err(EXIT_FAILURE, "could not allocate grid values");

    // go back to BOF, read values
    int c = g.height / 2 - 2; // centre point
    int x;
    int y = c;
    int z = c;
    int w = c;
    //fseek(f, 0, SEEK_SET);
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        x = c;
        char *s = linebuf;
        while (*s != '\n' && *s != '\0') {
            g.values[(w * g.time * g.height * g.width) + (z * g.height * g.width) + (g.width * y) + x] = *s++;
            x++;
        }
        y++;        
    }
    fclose(f);

    return g;
}

void 
print_grid(grid_t g) {
    int count = 0;
    for (int z=0; z < g.depth; z++) {
        printf("z=%d\n", z-100/2-2);
        for (int y=0; y < g.height; y++) {
            for (int x=0; x < g.width; x++) {
                if (g.values[(g.width * g.height * z) + (g.width * y) + x] == CH_ACTIVE) {
                    printf("#");
                    count++;
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
        printf("\n");
    }

    printf("count = %d\n", count);
}

/*
If a cube is active and exactly 2 or 3 of its neighbors are also active, 
    the cube remains active. 
Otherwise, the cube becomes inactive.

If a cube is inactive but exactly 3 of its neighbors are active, 
    the cube becomes active. 
Otherwise, the cube remains inactive.
*/
int 
count_active_neighbors(grid_t *g, int pos_x, int pos_y, int pos_z, int pos_w) {
    int count = 0;
    for (int w = pos_w - 1; w <= pos_w + 1; w++) {
        for (int z = pos_z - 1; z <= pos_z + 1; z++) {
            for (int y = pos_y - 1; y <= pos_y + 1; y++) {
                for (int x = pos_x - 1; x <= pos_x + 1; x++) {
                    // skip self
                    if (w == pos_w && z == pos_z && x == pos_x && y == pos_y) {
                        continue;
                    }

                    int idx = (w * g->depth * g->width * g->height) + (g->width * g->height * z) + (g->width * y) + x;
                    if (g->values[idx] == CH_ACTIVE) {
                        count++;
                    }
                }
            }
        }   
    }
    
    return count;
}

int
transmute_grid(grid_t *g) {
    char *values = (char *) calloc(CH_INACTIVE, g->time * g->depth * g->width * g->height * sizeof(char));
    if (!values) {
        err(EXIT_FAILURE, "error allocating memory for new grid values");
    }
    //memcpy(values, g->values, g->time * g->depth * g->width * g->height * sizeof(char));

    int idx;
    int active_neighbor_count;
    int count = 0;
    for (int w=1; w < g->time - 1; w++) {
        for (int z=1; z < g->depth - 1; z++) {
            for (int y=1; y < g->height  - 1; y++) {
                for (int x=1; x < g->width - 1; x++) {
                    idx = (w * g->depth * g->width * g->height) + (g->width * g->height * z) + (g->width * y) + x;
                    active_neighbor_count = count_active_neighbors(g, x, y, z, w);
                    switch (g->values[idx]) {
                        case CH_ACTIVE:
                            if (active_neighbor_count == 2 || active_neighbor_count == 3) {
                                values[idx] = CH_ACTIVE;
                                count++;
                            }
                        break;

                        default:
                        case CH_INACTIVE:
                            // If a cube is inactive but exactly 3 of its neighbors are active, 
                            // the cube becomes active.
                            if (active_neighbor_count == 3) {
                                values[idx] = CH_ACTIVE;
                                count++;
                            }
                        break;
                    }
                    
                }
            }
        }
    }

    free(g->values);
    g->values = values;
    return count;
}

int main() {
    grid_t g = read_input();  
    int count = 0;
    for (int i=0; i < 6; i++) {
        count = transmute_grid(&g);

        #ifdef STEP
        getchar();
        printf("Step %d\n", i+1);
        print_grid(g);
        #endif
    }

    printf("Count: %d\n", count);
    free(g.values);
}
