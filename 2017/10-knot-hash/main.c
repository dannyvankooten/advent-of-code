#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../adventofcode.h"

#define PUZZLE_NAME "Day 10: Knot Hash"

void reverse(int *values, int nvalues, int start, int length) {
    for (int i = 0; i < length/2; i++) {
        int idx_a = (start + i) % (nvalues);
        int idx_b = (start + length - i - 1) % (nvalues);
        int tmp = values[idx_a];
        values[idx_a] = values[idx_b];
        values[idx_b] = tmp;
    }
}

int pt1(char *s) {
    // parse
    int lengths[32];
    int nlengths = 0;
    while (*s != '\0') {
        s = parse_int(&lengths[nlengths++], s);
        if (*s == '\n' || *s == '\0') {
            break;
        }

        s = skip(",", s);
    }

    // knot
    int skip = 0;
    int cur = 0;
    int list[256];
    for (int i = 0; i < 256; i++) {
        list[i] = i;
    }

    for (int i = 0; i < nlengths; i++) {
        reverse(list, 256, cur, lengths[i]);
        cur += lengths[i] + skip;
        skip += 1;
    }

    return list[0] * list[1];
}

void pt2(char *hash, char *s) {
    // parse
    unsigned char lengths[64];
    int nlengths = 0;
    while (*s != '\0') {
        lengths[nlengths++] = *s++;
        if (*s == '\n' || *s == '\0') {
            break;
        }

        lengths[nlengths++] = *s++;
    }

    // add suffix
    memcpy(&lengths[nlengths], (unsigned char []){17, 31, 73, 47, 23}, 5 * sizeof(unsigned char));
    nlengths += 5;

    int skip = 0;
    int cur = 0;
    int list[256];
    for (int i = 0; i < 256; i++) {
        list[i] = i;
    }
    for (int r = 0; r < 64; r++) {
        for (int i = 0; i < nlengths; i++) {
            reverse(list, 256, cur, lengths[i]);
            cur += lengths[i] + skip;
            skip += 1;
        }
    }

    // write dense hash values to char array
    for (int i = 0; i < 16; i++) {
        uint8_t h = 0;
        for (int j = 0; j < 16; j++) {
            h ^= list[i*16+j];
        }
        sprintf(hash + (i * 2), "%02x", h);
    }
    hash[32] = 0x0;
}

int main() {
    clock_t t = timer_start();
    char input[1024 * 64];
    read_input_file(input, "input.txt");

    int a1 = pt1(input);
    char hash[33];
    pt2(hash, input);
    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", a1);
    printf("Part 2: %s\n", hash);
    printf("Time: %.2fms\n", timer_stop(t));
    return EXIT_SUCCESS;
}
