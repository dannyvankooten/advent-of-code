#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void read_input(char *dst) {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading input.txt\n");
        abort();
    }
    size_t nread = fread(dst, 1, 64 * 1024, fp);
    fclose(fp);
    dst[nread++] = '\0';
}

void solve(char *b, char *s, char *const *grid, int gridsize) {
    int x = (gridsize - 1) / 2;
    int y = x;

    while (*s != '\0') {
        switch (*s) {
            case '\n':
                *b++ = grid[y][x];
                break;

            case 'U':
                if (y > 0 && grid[y - 1][x] != ' ') {
                    y--;
                }
                break;

            case 'L':
                if (x > 0 && grid[y][x - 1] != ' ') {
                    x--;
                }
                break;

            case 'R':
                if (x < (gridsize - 1) && grid[y][x + 1] != ' ') {
                    x++;
                }
                break;

            case 'D':
                if (y < (gridsize - 1) && grid[y + 1][x] != ' ') {
                    y++;
                }
                break;
        }

        s++;
    }

    *b = '\0';
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[64 * 1024];
    read_input(input);

    printf("--- Day 2: Bathroom Security ---\n");
    char buf[16];

    char *grid1[] = {
        "123",
        "456",
        "789",
    };
    solve(buf, input, grid1, 3);
    printf("Part 1: %s\n", buf);

    char *grid2[] = {
            "  1  ",
            " 234 ",
            "56789",
            " ABC ",
            "  D  ",
    };
    solve(buf, input, grid2, 5);
    printf("Part 1: %s\n", buf);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
}
