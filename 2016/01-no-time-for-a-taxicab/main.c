#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

char *parse_uint8(uint8_t *dest, char *s) {
    uint8_t n = 0;
    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }

    *dest = n;
    return s;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[64*1024];
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    size_t nread = fread(input, 1, 64*1024, fp);
    input[nread] = '\0';
    fclose(fp);

    char *s = input;
    int pos[] = {0, 0};
    int dirs[][2] = {
        {0, 1},     // N
        {1, 0},     // E
        {0, -1},    // S
        {-1, 0},    // W
    };
    int cdir = 0;   // N

    char dir;
    uint8_t amount;
    uint8_t seen[256*256] = {0};
    int pt2 = -1;

    while (*s != '\0') {
        dir = *s++;
        s = parse_uint8(&amount, s);

        // skip comma and space
        while (*s == ',' || *s == ' ' || *s == '\n') s++;

        if (dir == 'R') {
            cdir = (cdir == 3) ? 0 : cdir + 1;
        } else {
            cdir = (cdir == 0) ? 3 : cdir - 1;
        }

        while (amount > 0) {
            pos[0] += dirs[cdir][0];
            pos[1] += dirs[cdir][1];
            amount--;

            if (pt2 == -1) {
                int idx = (pos[0] + 128) * 256 + (pos[1] + 128);
                if (seen[idx] == 1) {
                    pt2 = abs(pos[0]) + abs(pos[1]);
                }

               seen[idx] = 1;
            }
        }
    }

    printf("--- Day 1: No Time for a Taxicab ---\n");
    fprintf(stdout, "Part 1: %d\n", abs(pos[0]) + abs(pos[1]));
    fprintf(stdout, "Part 2: %d\n", pt2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}
