#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define PUZZLE_NAME "Day 5: A Maze of Twisty Trampolines, All Alike"

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

char *parse_int(int *restrict dst, char *s) {
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

int parse_input(int dst[], char *restrict input) {
    char *s = input;
    int n = 0;
    while (*s != '\0') {
        int d;
        s = parse_int(&d, s);
        dst[n++] = d;
        if (*s == '\n') s++;
    }

    return n;
}

int pt1(int _jumps[], int njumps) {
    int jumps[2048];
    memcpy(jumps, _jumps, 2048 * sizeof(int));
    int i = 0;
    int steps, offset;
    for ( steps = 0; i < njumps && i >= 0; steps++) {
        offset = jumps[i];
        jumps[i]++;
        i += offset;
    }
    return steps;
}

int pt2(int jumps[], int njumps) {
    int i = 0;
    int steps, offset;
    for ( steps = 0; i < njumps && i >= 0; steps++) {
        offset = jumps[i];
        jumps[i] += (offset >= 3) ? -1 : 1;
        i += offset;
    }
    return steps;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[1024 * 64];
    read_input_file(input, "input.txt");

    int jumps[2048];
    int njumps = parse_input(jumps, input);

    int a1 = pt1(jumps, njumps);
    int a2 = pt2(jumps, njumps);
    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", a1);
    printf("Part 2: %d\n", a2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return EXIT_SUCCESS;
}
