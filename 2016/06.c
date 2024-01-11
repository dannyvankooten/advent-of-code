#include "adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 6: Signals and Noise"

void pt1(char *pt1, char *pt2, char *input) {
    int counts[8][26];
    memset(counts, 0, 26 * 8 * sizeof(int));
    char *s = input;
    int col = 0;
    while (*s != 0x0) {
        counts[col++][*s++ - 'a']++;

        if (*s == '\n') {
            s++;
            col = 0;
        }
    }

    for (col = 0; col < 8; col++) {
        char max = 'a';
        char least = 'a';
        for (char ch = 'b'; ch <= 'z'; ch++) {
            if (counts[col][ch - 'a'] > counts[col][max - 'a']) {
                max = ch;
            }

            if (counts[col][ch - 'a'] < counts[col][least - 'a']) {
                least = ch;
            }
        }
        pt1[col] = max;
        pt2[col] = least;
    }
    pt1[9] = 0x0;
    pt2[9] = 0x0;
}

int main() {
    clock_t t = clock_time();
    char input[1024 * 64] = "";
    read_input_file(input, 1024 * 64, "05.txt");

    char a1[32];
    char a2[32];
    pt1(a1, a2, input);

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %s\n", a1);
    printf("Part 2: %s\n", a2);
    printf("Time: %.2fms\n", clock_time_since(t));
    return EXIT_SUCCESS;
}
