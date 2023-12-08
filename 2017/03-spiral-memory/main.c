#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define W 1024

char *dirnames[] = {
    "RIGHT",
    "UP",
    "LEFT",
    "DOWN",
};

int pt1() {
    const int center = W/2;
    const char directions[][2] = {
    {0, 1}, // right
    {-1, 0}, // up
    {0, -1}, // left
    {1, 0}, // down
    };
    int grid[W*W];
    grid[center*W+center] = 1;
    for (int i = 2, steps = 1, x = center, y = center, d = 0;;) {
        // spiral grid until we're entering the value we're looking for
        for (int k = 0; k < 2; k++) {
            for (int j = 0; j < steps; j++) {
                if (i == 277678) {
                    return abs(y-center)+abs(x-center);
                }

                y += directions[d][0];
                x += directions[d][1];
                grid[y * W + x] = i++;
            }
            d = d == 3 ? 0 : d + 1;
        }

        steps += 1;
    }

    return -1;
}

int sum_neighbors(int *grid, int y, int x) {
    const char directions[][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1},
    {1, -1}, {1, 0}, {1, 1},
    };
    int sum = 0;
    for (int i = 0; i < 8; i++) {
        int y2 = y + directions[i][0];
        int x2 = x + directions[i][1];

        if (y2 < 0 || x2 < 0 || y2 > W-1 || x2 > W-1) {
            continue;
        }

        sum += grid[y2*W+x2];
    }
    return sum;
}

int pt2() {
    int grid[W*W];
    memset(grid, 0, sizeof(int) * W * W);
    const int center = W/2;
    const char directions[][2] = {
    {0, 1}, // right
    {-1, 0}, // up
    {0, -1}, // left
    {1, 0}, // down
    };
    grid[center*W+center] = 1;

    for (int steps = 1, x = center, y = center, d = 0, sum;;) {
        // spiral grid until we're entering the value we're looking for
        for (int k = 0; k < 2; k++) {
            for (int j = 0; j < steps; j++) {
                y += directions[d][0];
                x += directions[d][1];
                sum = sum_neighbors(grid, y, x);
                if (sum > 277678) {
                    return sum;
                }
                grid[y * W + x] = sum;
            }
            d = d == 3 ? 0 : d + 1;
        }

        steps += 1;
    }

    return -1;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    printf("--- Day 3: Spiral Memory ---\n");
    printf("Part 1: %d\n", pt1());
    printf("Part 2: %d\n", pt2());

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return EXIT_SUCCESS;
}
