#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define PUZZLE_NAME "Day 7: Recursive Circus"

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

typedef struct {
    char name[16];
    int weight;
    char children[8][16];
    int nchildren;
} program_t;

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

int parse_input(program_t *programs, char *input) {
    char *s = input;
    int n = 0;
    char *w;
    while (*s != '\0') {
        programs[n].nchildren = 0;
        w = programs[n].name;
        while (*s != ' ') {
            *w++ = *s++;
        }
        *w = '\0';

        // skip space
        s += 2;
        s = parse_int(&programs[n].weight, s);
        s++;

        if (*s != '\n') {
            s += 4;

            // parse children
            while (*s != '\n') {
                w = programs[n].children[programs[n].nchildren];
                while (*s >= 'a' && *s <= 'z') {
                    *w++ = *s++;
                }
                *w = '\0';
                programs[n].nchildren++;
                if (*s == ',') s += 2;
            }
        }

        while (*s != '\n') s++;
        s++;

        n++;
    }

    return n;
}

unsigned char isroot(program_t *node, program_t *programs, int nprograms) {
    for (int j = 0; j < nprograms; j++) {
        for (int c = 0; c < programs[j].nchildren; c++) {
            if (strcmp(node->name, programs[j].children[c]) == 0) {
                return 0;
            }
        }
    }

    return 1;
}

char *pt1(program_t *programs, int nprograms) {
    for (int i = 0; i < nprograms; i++) {
        if (isroot(&programs[i], programs, nprograms)) {
            return programs[i].name;
        }
    }

    return NULL;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[1024 * 64];
    read_input_file(input, "input_test.txt");

    program_t *programs = malloc(2048 * sizeof(program_t));
    int nprograms = parse_input(programs, input);

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %s\n", pt1(programs, nprograms));
    printf("Part 2: %d\n", 0);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return EXIT_SUCCESS;
}
