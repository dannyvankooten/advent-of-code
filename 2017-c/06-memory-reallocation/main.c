#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "../hashmap.h"

#define PUZZLE_NAME "Day 6: Memory Reallocation"

typedef struct answer {
    int pt1;
    int pt2;
} answer_t ;

void read_input_file(char *dest, char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        fprintf(stderr, "error reading %s", file);
        exit(EXIT_FAILURE);
    }
    size_t nread = fread(dest, 1, 64*1024, fp);
    dest[nread] = '\0';
    fclose(fp);
}

static inline
int imax(const uint8_t *restrict mem, int nmem) {
    int imax = 0;
    for (int i = 1; i < nmem; i++) {
        if (mem[i] > mem[imax]) {
            imax = i;
        }
    }
    return imax;
}

answer_t pt1(uint8_t *restrict mem, int nmem) {
    hashmap_t hm = hashmap_new(2 << 14);
    hashmap_set(&hm, mem, 16, 0);

    int i, j, m, blocks;
    for (i = 1; ; i++) {
        // distribute blocks
        m = imax(mem, nmem);
        blocks = mem[m];
        for (j = m+1; blocks > 0; j++) {
            if (j == nmem) j = 0;

            mem[j]++;
            mem[m]--;
            blocks--;
        }

        // have we seen this state before?
        int *seen = hashmap_get(&hm, mem, 16);
        if (seen != NULL) {
            answer_t a = (answer_t ){
                i,
                i - *seen,
            };
            hashmap_free(&hm);
            return a;
        }

        hashmap_set(&hm, mem, 16, i);
    }

}

char *parse_uint8(uint8_t *restrict dst, char *s) {
    int mod = 1;
    int n = 0;
    if (*s == '-') {
        s++;
        mod = -1;
    }

    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }
    n *= mod;
    *dst = n;
    return s;
}

int parse_input(uint8_t *dst, char *s) {
    int n = 0;
    while (*s != '\n' && *s != '\0') {
        s = parse_uint8(&dst[n++], s);
        while (*s == ' ' || *s == '\t') s++;
    }
    return n;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[1024 * 64];
    read_input_file(input, "input.txt");

    uint8_t mem[16];
    int nmem = parse_input(mem, input);

    answer_t a = pt1(mem, nmem);
    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", a.pt1);
    printf("Part 2: %d\n", a.pt2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return EXIT_SUCCESS;
}
