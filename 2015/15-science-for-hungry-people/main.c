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

typedef struct ingredient {
    char name[14];
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
} ingredient_t;



void parse(ingredient_t *ingredients, int *ningredients) {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    char input[64 * 1024];
    size_t nread = fread(input, 1, 1024*32, fp);
    fclose(fp);
    input[nread] = '\0';

    int n = 0;
    char *s = input;
    while (*s != '\0') {
        s = parse_ident(ingredients[n].name, s);
        while ((*s < '0' || *s > '9') && *s != '-') s++;
        s = parse_int(&ingredients[n].capacity, s);
        while ((*s < '0' || *s > '9') && *s != '-') s++;
        s = parse_int(&ingredients[n].durability, s);
        while ((*s < '0' || *s > '9') && *s != '-') s++;
        s = parse_int(&ingredients[n].flavor, s);
        while ((*s < '0' || *s > '9') && *s != '-') s++;
        s = parse_int(&ingredients[n].texture, s);
        while ((*s < '0' || *s > '9') && *s != '-') s++;
        s = parse_int(&ingredients[n].calories, s);

        while (*s != '\n' && *s != '\0') s++;
        if (*s == '\n') s++;

        n++;
    }

    *ningredients = n;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    ingredient_t ingredients[4];
    int ningredients = 0;
    parse(ingredients, &ningredients);

    int capacity;
    int durability;
    int flavor;
    int texture;
    int score;
    int calories;
    int max_score = -1;
    int max_score_500c = -1;
    for (int i = 0; i < 100; i++) {
        for (int k=0; k < 100 - i; k++) {
            for (int l=0; l < 100 - k; l++) {
                for (int m=0; m < 100 - l; m++) {
                    if (i + k + l + m != 100) continue;

                    capacity = ingredients[0].capacity * i + ingredients[1].capacity * k + ingredients[2].capacity * l + ingredients[3].capacity * m;
                    if (capacity <= 0) continue;

                    durability = ingredients[0].durability * i + ingredients[1].durability * k + ingredients[2].durability * l + ingredients[3].durability * m;
                    if (durability <= 0) continue;

                    flavor = ingredients[0].flavor * i + ingredients[1].flavor * k + ingredients[2].flavor * l + ingredients[3].flavor * m;
                    if (flavor <= 0) continue;

                    texture = ingredients[0].texture * i + ingredients[1].texture * k + ingredients[2].texture * l + ingredients[3].texture * m;
                    if (texture <= 0) continue;

                    score = capacity * durability * flavor * texture;
                    if (score > max_score) {
                        max_score = score;
                    }

                    calories = ingredients[0].calories * i + ingredients[1].calories * k + ingredients[2].calories * l + ingredients[3].calories * m;
                    if (calories == 500 && score > max_score_500c) {
                        max_score_500c = score;
                    }
                }
            }
        }
    }

    printf("--- Day 15: Science for Hungry People ---\n");
    printf("Part 1: %d\n", max_score);
    printf("Part 2: %d\n", max_score_500c);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}