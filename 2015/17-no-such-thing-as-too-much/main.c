#include <stdio.h>
#include <string.h>
#include <time.h>

int min_containers = 1 << 30;
int pt2 = 0;

int permute(int sizes[], int flags, int i, int n) {
    if (i == n) {
        int s = 0;
        int c = 0;
        int b;
        for (int x = 0; x < n; x++) {
            b = (int) ((flags >> (n-x)) & 1);
            c += b;
            s += sizes[x] *b;
        }

        if (s == 150) {
            if (c < min_containers) {
                min_containers = c;
                pt2 = 1;
            } else if (c == min_containers) {
                pt2++;
            }

            return 1;
        }
        return 0;
    }

    // try to bail early
    int s = 0;
    for (int x = 0; x < i; x++) {
        s += sizes[x] * (int) ((flags >> (n-x)) & 1);
    }
    if (s > 150) {
        return 0;
    }

    return permute(sizes, flags, i+1, n) + permute(sizes, flags | 1 << (n-i), i+1, n);
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    printf("--- Day 17: No Such Thing as Too Much ---\n");

    int sizes[] = {
        47,
        46,
        44,
        44,
        43,
        41,
        38,
        36,
        34,
        31,
        27,
        21,
        17,
        17,
        10,
        9,
        6,
        4,
        4,
        3
    };
    int flags = 0;
    int count = permute(sizes, flags,0, 20);

    printf("Part 1: %d\n", count);
    printf("Part 2: %d\n", pt2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}