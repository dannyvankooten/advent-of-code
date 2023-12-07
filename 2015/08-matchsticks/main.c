#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>


int main() {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    char input[20*1024];
    size_t nread = fread(input, 1, 20*1024, fp);
    input[nread] = '\0';
    fclose(fp);

    clock_t start_t, end_t;
    start_t = clock();

    char *s = input;
    int pt1 = 0;
    int pt2 = 0;

    while (*s != '\0') {
        // skip opening "
        s++;

        // opening and closing ""
        pt1 += 2;
        pt2 += 4;

        while (*s != '"') {
             if (*s == '\\') {
                 // hexadecimal ASCII
                 if (*(s+1) == 'x') {
                     s += 4;
                     pt1 += 3;
                     pt2 += 1;
                 } else {
                     // single escaped character
                     s += 2;
                     pt1 += 1;
                     pt2 += 2;
                }

                continue;
            }

            // normal character
            s++;
        }

        // skip closing "
        s++;

        // skip newline
        s++;
    }

    printf("--- Day 8: Matchsticks ---\n");
    printf("Part 1: %d\n", pt1);
    assert(pt1 == 1342);

    printf("Part 2: %d\n", pt2);
    assert(pt2 == 2074);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}
