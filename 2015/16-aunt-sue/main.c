#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

static inline
char *parse_int(int *dst, char *s) {
    int mod = 1;
    int n = 0;

    if (*s == '-') {
        mod = -1;
        s++;
    }

    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }
    *dst = n * mod;
    return s;
}

static inline
char *parse_ident(char *dst, char *s) {
    while (*s != ':') {
        *dst++ = *s++;
    }
    *dst++ = '\0';
    return s;
}

int pt1(char *input) {
    int n = 0;
    char ident[32];
    int amount;

    char *s = input;
    while (*s != '\0') {
        while (*s != ':') s++;
        s++;

        while (*s != '\n') {
            s = parse_ident(ident, s);
            s += 2; // ": "
            s = parse_int(&amount, s);
            if (*s == ',') s += 2;

            if (strcmp(ident, "children") == 0 && amount != 3) {
                goto skip;
            } else if (strcmp(ident, "cats") == 0 && amount != 7) {
                goto skip;
            } else if (strcmp(ident, "samoyeds") == 0 && amount != 2) {
                goto skip;
            } else if (strcmp(ident, "pomeranians") == 0 && amount != 3) {
                goto skip;
            } else if (strcmp(ident, "akitas") == 0 && amount != 0) {
                goto skip;
            } else if (strcmp(ident, "vizslas") == 0 && amount != 0) {
                goto skip;
            } else if (strcmp(ident, "goldfish") == 0 && amount != 5) {
                goto skip;
            } else if (strcmp(ident, "trees") == 0 && amount != 3) {
                goto skip;
            } else if (strcmp(ident, "cars") == 0 && amount != 2) {
                goto skip;
            } else if (strcmp(ident, "perfumes") == 0 && amount != 1) {
                goto skip;
            }
        }

        return n+1;

        skip:
        while (*s != '\n' && *s != '\0') s++;
        if (*s == '\n') s++;
        n++;
    }

    return -1;
}


int pt2(char *input) {
    int n = 0;
    char ident[32];
    int amount;

    char *s = input;
    while (*s != '\0') {
        while (*s != ':') s++;
        s += 2; // ": "

        while (*s != '\n') {
            s = parse_ident(ident, s);
            if (*s != ':') abort();
            s += 2; // ": "
            s = parse_int(&amount, s);
            if (*s == ',') s += 2; // ", "

            if (
                (strcmp(ident, "children") == 0 && amount != 3)
                || (strcmp(ident, "cats") == 0 && amount <= 7)
                || (strcmp(ident, "samoyeds") == 0 && amount != 2)
                || (strcmp(ident, "pomeranians") == 0 && amount >= 3)
                || (strcmp(ident, "akitas") == 0 && amount != 0)
                || (strcmp(ident, "vizslas") == 0 && amount != 0)
                || (strcmp(ident, "goldfish") == 0 && amount >= 5)
                || (strcmp(ident, "trees") == 0 && amount <= 3)
                || (strcmp(ident, "cars") == 0 && amount != 2)
                || (strcmp(ident, "perfumes") == 0 && amount != 1)
            ){
                goto skip;
            }
        }

        return n+1;
        skip:

        while (*s != '\n' && *s != '\0') s++;
        if (*s == '\n') s++;
        n++;
    }

    return -1;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();
    FILE *fp = fopen("input.txt", "r");
    char input[64 * 1024];
    size_t nread = fread(input, 1, 1024*32, fp);
    fclose(fp);
    input[nread] = '\0';

    int a1 = pt1(input);
    printf("part 1: %d\n", a1);
    assert(a1 == 213);

    int a2 = pt2(input);
    printf("part 2: %d\n", a2);
    assert(a2 == 323);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("%.2fms\n", total_t);
    return 0;
}