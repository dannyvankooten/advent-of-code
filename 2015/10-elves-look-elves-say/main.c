#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>

int apply(char *dst, char *in) {
    char *s = in;
    char *o = dst;
    int d;

    while (*s != '\0') {
        d = 1;
        for (;s[d] == *s; d++);

        *o++ = d + '0';
        *o++ = *s;
        s += d;
    }

    *o = '\0';
    return o - dst;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    // single malloc call for 16MB of memory
    char *input = (char *) malloc(1024*1024*16);
    if (input == NULL) {
        printf("error allocating enough memory for string buffers");
        exit(EXIT_FAILURE);
    }
    char *buf = input + (1024*1024*8 * sizeof(char));
    char *tmp;

    printf("--- Day 10: Elves Look, Elves Say ---\n");

    strcpy(input, "1321131112");
    int pt1;
    for (int i = 0; i < 40; i++) {
        pt1 = apply(buf, input);
        tmp = input;
        input = buf;
        buf = tmp;
    }
    printf("Part 1: %d\n", pt1);
    assert(pt1 == 492982);

    int pt2;
    for (int i = 0; i < 10; i++) {
        pt2 = apply(buf, input);
        tmp = input;
        input = buf;
        buf = tmp;
    }
    printf("Part 2: %d\n", pt2);
    assert(pt2 == 6989950);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);

    free(input);
    return 0;
}