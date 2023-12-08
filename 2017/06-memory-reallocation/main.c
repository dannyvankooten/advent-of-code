#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define PUZZLE_NAME "Day 6: Memory Reallocation"

void read_input_file(char *dest, char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    size_t nread = fread(dest, 1, 64*1024, fp);
    dest[nread] = '\0';
    fclose(fp);
}

int solve(char *s) {
    return 0;
}

/*
 0 2 7 0        start
       v
 0 2 6 1        1
 v
 1 2 5 1        2
   v
 1 3 4 1        3
     v
 1 3 4 1        4
       v
 1 3 3 2        5
 v
 2 3 2 2        6
   v
 2 4 1 2        7
 */

int imax(u_int8_t mem[], int nmem) {
    int imax = 0;
    for (int i = 1; i < nmem; i++) {
        if (mem[i] > mem[imax]) {
            imax = i;
        }
    }
    return imax;
}

int pt1(u_int8_t mem[], int nmem) {
    u_int8_t seen[1 << 14][nmem];
    int nseen = 0;
    memcpy(seen[nseen++], mem, nmem * sizeof(u_int8_t));

    for (int i = 1; ; i++) {
        int m = imax(mem, nmem);
        int blocks = mem[m];
        for (int j = m+1; blocks > 0; j++) {
            if (j == nmem) j = 0;

            mem[j]++;
            mem[m]--;
            blocks--;
        }

        // have we seen this state before?
        for (int j = 0; j < nseen; j++) {
            if (memcmp(seen[j], mem, sizeof(u_int8_t) * nmem) == 0) {
                return i;
            }
        }

        memcpy(seen[nseen++], mem, nmem * sizeof(u_int8_t));
    }
}

char *parse_uint8(u_int8_t *restrict dst, char *s) {
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

int parse_input(u_int8_t *dst, char *s) {
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

    u_int8_t mem[16];
    int nmem = parse_input(mem, input);

    int a1 = pt1(mem, nmem);
    int a2 = pt1(mem, nmem);
    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", a1);
    printf("Part 2: %d\n", a2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return EXIT_SUCCESS;
}
