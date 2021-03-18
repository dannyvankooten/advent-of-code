#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int SQ_TREE = 1;
const int SQ_OPEN = 0;
const int HEIGHT = 322;
const int WIDTH = 31;
unsigned long count_trees(short grid[][WIDTH], int slope_y, int slope_x);

int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) return 1;

    char buf[BUFSIZ];
   
    short grid[HEIGHT][WIDTH];
    for (int y=0; y<HEIGHT; y++) {
        for (int x=0; x<WIDTH; x++) {
            grid[y][x] = 0;
        }
    }
    int y = 0;
    int x = 0;

    while (fgets(buf, BUFSIZ, f) != NULL) {
        char *str = buf;
        while (*str != '\n' && *str != '\0') {
            grid[y][x++] = *str == '#' ? SQ_TREE : SQ_OPEN;
            str++;
        }

        y++;
        //printf("y = %d\tx = %d\n", y, x);
        x = 0;
    }

    fclose(f);

    // print grid
    for (int y=0; y < HEIGHT; y++) {
        for (int x=0; x < WIDTH; x++) {
            printf("%c", grid[y][x] == SQ_OPEN ? '.' : '#');
        }
        printf("\n");
    }

    // last x, y
    printf("\n\nLast: %d\n", grid[HEIGHT-1][WIDTH-1]);

    unsigned long long product = count_trees(grid, 1, 1) 
        * count_trees(grid, 1, 3) 
        * count_trees(grid, 1, 5)
        * count_trees(grid, 1, 7)
        * count_trees(grid, 2, 1);

    printf("%lld trees\n", product);
}

unsigned long count_trees(short grid[][WIDTH], int slope_y, int slope_x) {
    unsigned long tree_count = 0;
    for (int y=0, x = 0; y < 322; y += slope_y, x += slope_x) {
        if (x >= 31) {
            x -= 31;
        }

        if (grid[y][x] == SQ_TREE) {
            tree_count++;
        }
    }
    return tree_count;
}