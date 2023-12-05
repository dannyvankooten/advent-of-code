#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>

void apply(char *dst, char *in) {
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
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char *input = malloc(1024*1024*8);
    char *buf = malloc(1024*1024*8);

    strcpy(input, "1321131112");
    for (int i = 0; i < 40; i++) {
        apply(buf, input);
        strcpy(input, buf);
    }
    int pt1 = strlen(input);
    printf("part 1: %d\n", pt1);
    assert(pt1 == 492982);

    for (int i = 0; i < 10; i++) {
        apply(buf, input);
        strcpy(input, buf);
    }
    int pt2 = strlen(input);
    printf("part 2: %d\n", pt2);
    assert(pt2 == 6989950);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("%.2fms\n", total_t);
    return 0;
}