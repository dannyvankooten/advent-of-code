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
    char *values;
};
typedef struct grid grid_t;

grid_t
read_input() {
    char linebuf[BUFSIZ] = {0};
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    
    // // first, read width
    // fgets(linebuf, BUFSIZ, f);
    // int width = strlen(linebuf) - 1;

    // // read height
    // int height = 1;
    // while(fgets(linebuf, BUFSIZ, f) != NULL) height++;

    // assume high enough values to fit our infinite grid
    grid_t g = {
        .width = 32,
        .height = 32,
        .depth = 32,
    };
    g.values = (char *) malloc(g.depth * g.width * g.height * sizeof(char));
    if (!g.values) err(EXIT_FAILURE, "could not allocate grid values");

    // go back to BOF, read values
    int x;
    int y = g.height / 2 - 2;
    int z = g.depth / 2 - 2;
    //fseek(f, 0, SEEK_SET);
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        x = g.width / 2 - 2;
        char *s = linebuf;
        while (*s != '\n' && *s != '\0') {
            g.values[(z * g.height * g.width) + (g.width * y) + x] = *s++;
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
count_active_neighbors(grid_t *g, int pos_x, int pos_y, int pos_z) {
    int z_start = pos_z - 1;
    int z_end = pos_z + 1;
    int x_start = pos_x - 1;
    int x_end = pos_x + 1;
    int y_start = pos_y - 1;
    int y_end = pos_y + 1;
    int count = 0;
    for (int z=z_start; z <= z_end; z++) {
        for (int y=y_start; y <= y_end; y++) {
            for (int x=x_start; x <= x_end; x++) {
                // skip self
                if (z == pos_z && x == pos_x && y == pos_y) {
                    continue;
                }

                int idx = (g->width * g->height * z) + (g->width * y) + x;
                if (g->values[idx] == CH_ACTIVE) {
                    count++;
                }
            }
        }
    }   
    
    return count;
}

void
transmute_grid(grid_t *g) {
    char *values = (char *) malloc(g->depth * g->width * g->height * sizeof(char));
    if (!values) {
        err(EXIT_FAILURE, "error allocating memory for new grid values");
    }
    memcpy(values, g->values, g->depth * g->width * g->height * sizeof(char));

    // TODO: MAGIC
    for (int z=1; z < g->depth - 1; z++) {
        for (int y=1; y < g->height  - 1; y++) {
            for (int x=1; x < g->width - 1; x++) {
                int idx = (g->width * g->height * z) + (g->width * y) + x;
                char state = g->values[idx];
                int active_neighbor_count = count_active_neighbors(g, x, y, z);
                switch (state) {
                    case CH_ACTIVE:
                        if (active_neighbor_count < 2 || active_neighbor_count > 3) {
                            values[idx] = CH_INACTIVE;
                        }
                    break;

                    default:
                    case CH_INACTIVE:
                        // If a cube is inactive but exactly 3 of its neighbors are active, 
                        // the cube becomes active.
                        if (active_neighbor_count == 3) {
                             values[idx] = CH_ACTIVE;
                        }
                    break;
                }
                
            }
        }
    }


    free(g->values);
    g->values = values;
}

int main() {
    grid_t g = read_input();  
    printf("w=%d\th=%d\td=%d\n\n", g.width, g.height, g.depth);

    for (int i=0; i < 6; i++) {
        transmute_grid(&g);

        #ifdef STEP
        getchar();
        printf("Step %d\n", i+1);
        print_grid(g);
        #endif
    }

    int count = 0;
    for (int z=0; z < g.depth; z++) {
        for (int y=0; y < g.height; y++) {
            for (int x=0; x < g.width; x++) {
                int idx = (g.width * g.height * z) + (g.width * y) + x;
                if (g.values[idx] == CH_ACTIVE) {
                    count++;
                }
            }
        }
    }
    printf("Count: %d\n", count);

    free(g.values);
}
