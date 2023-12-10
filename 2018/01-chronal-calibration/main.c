#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../adventofcode.h"

#define PUZZLE_NAME "Day 1: Chronal Calibration"

int pt1(char *s) {
    int freq = 0;
    while (*s != '\0') {
        int mod = *s++ == '-' ? -1 : 1;

        int n = 0;
        while (*s >= '0' && *s <= '9') {
            n = (n * 10) + (*s++ - '0');
        }

        freq += (mod * n);

        // skip \n
        s++;
    }
    return freq;
}

int pt2(char *input) {
    int freq = 0;
    uint8_t seen[256*1024] = {0};

    for (;;) {
        char *s = input;
        int mod;
        int hash;
        int n;

        while (*s != '\0') {
            mod = *s++ == '-' ? -1 : 1;
            n = 0;
            while (*s >= '0' && *s <= '9') {
                n = (n * 10) + (*s++ - '0');
            }

            freq += (mod * n);
            hash = freq + 128 * 1024;
            if (seen[hash] == 1) {
                return freq;
            }
            seen[hash] = 1;

            // skip \n
            s++;
        }
    }
}

int main() {
    clock_t t = timer_start();
    char input[1024 * 64];
    read_input_file(input, "input.txt");

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", pt1(input));
    printf("Part 2: %d\n", pt2(input));
    printf("Time: %.2fms\n", timer_stop(t));
    return EXIT_SUCCESS;
}
