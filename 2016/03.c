#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *parse_int(int *dest, char *s) {
    int n = 0;
    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }

    *dest = n;
    return s;
}

void read_input(char *dst) {
    FILE *fp = fopen("03.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    size_t nread = fread(dst, 1, 64 * 1024, fp);
    fclose(fp);
    dst[nread] = '\0';
}

int pt1(char *s) {
    int n = 0;
    int a, b, c;

    while (*s != '\0') {
        while (*s == ' ')
            s++;
        s = parse_int(&a, s);
        while (*s == ' ')
            s++;
        s = parse_int(&b, s);
        while (*s == ' ')
            s++;
        s = parse_int(&c, s);
        while (*s != '\n' && *s != '\0')
            s++;
        if (*s == '\n')
            s++;

        if (a + b > c && a + c > b && b + c > a) {
            n++;
        }
    }

    return n;
}

int pt2(char *s) {
    int n = 0;
    int sides[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    int i = 0;
    while (*s != '\0') {
        while (*s == ' ')
            s++;
        s = parse_int(&sides[i], s);
        while (*s == ' ')
            s++;
        s = parse_int(&sides[i + 3], s);
        while (*s == ' ')
            s++;
        s = parse_int(&sides[i + 6], s);
        while (*s != '\n' && *s != '\0')
            s++;
        if (*s == '\n')
            s++;

        if (i == 2) {
            for (; i >= 0; i--) {
                if (sides[0 + i * 3] + sides[1 + i * 3] > sides[2 + i * 3] &&
                    sides[0 + i * 3] + sides[2 + i * 3] > sides[1 + i * 3] &&
                    sides[1 + i * 3] + sides[2 + i * 3] > sides[0 + i * 3]) {
                    n++;
                }
            }
            i = 0;
        } else {
            i++;
        }
    }

    return n;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[64 * 1024];
    read_input(input);

    printf("--- Day 3: Squares With Three Sides ---\n");
    printf("Part 1: %d\n", pt1(input));
    printf("Part 2: %d\n", pt2(input));

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
}
