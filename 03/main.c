#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

const int_fast8_t SQ_TREE = 1;
const int_fast8_t SQ_OPEN = 0;
const int HEIGHT = 323;
const int WIDTH = 31;
unsigned long count_trees(int_fast8_t *grid, int slope_y, int slope_x);

int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        return 1;
    }
    char buf[BUFSIZ] = {0};
    int_fast8_t *grid = calloc(HEIGHT * WIDTH, sizeof(int_fast8_t));
    int y = 0;
    int x = 0;

    while (fgets(buf, BUFSIZ, f) != NULL) {
        x = 0;
        char *str = buf;
        while (*str != '\n' && *str != '\0') {
            grid[y * WIDTH + x] = (*str == '#') ? SQ_TREE : SQ_OPEN;
            str++;
            x++;
        }

        y++;
    }
    fclose(f);

    // // print grid
    // for (int y=0; y < HEIGHT; y++) {
    //     for (int x=0; x < WIDTH; x++) {
    //         printf("%c", grid[y][x] == SQ_OPEN ? '.' : '#');
    //     }
    //     printf("\n");
    // }

    // last x, y
    // printf("\n\nLast: %d\n", grid[(HEIGHT-1) * WIDTH + WIDTH-1]);

    int64_t product = count_trees(grid, 1, 1) 
        * count_trees(grid, 1, 3) 
        * count_trees(grid, 1, 5)
        * count_trees(grid, 1, 7)
        * count_trees(grid, 2, 1);

    printf("%" PRId64 " trees\n", product);

    free(grid);
}

unsigned long count_trees(int_fast8_t *grid, int slope_y, int slope_x) {
    unsigned long tree_count = 0;
    for (int y=0, x = 0; y < 322; y += slope_y, x += slope_x) {
        if (x >= 31) {
            x -= 31;
        }

        if (grid[y * WIDTH + x] == SQ_TREE) {
            tree_count++;
        }
    }
    return tree_count;
}