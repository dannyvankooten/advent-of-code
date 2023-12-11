#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../adventofcode.h"

#define PUZZLE_NAME "Day 2: Inventory Management System"

int pt1(char *s) {
    uint8_t counts[26] = {0};
    int twos = 0;
    int threes = 0;

    while (*s != '\0') {
        if (*s == '\n') {
            // find twos and threes
            uint8_t c2 = 0;
            uint8_t c3 = 0;
            for (uint8_t i=0; i < 26; i++) {
                if (counts[i] == 2) {
                    c2 = 1;
                } else if (counts[i] == 3) {
                    c3 = 1;
                }
            }

            memset(counts, 0, 26 * sizeof(uint8_t));
            twos += c2;
            threes += c3;
        } else {
            counts[(*s)-'a']++;
        }

        s++;
    }

    return twos * threes;
}

void pt2(char *common, char *s) {
    // 27 chars per line (incl. trailing \n)
    // 250 lines
    // 25 must match
    int ncommon = 0;
    for (uint8_t i = 0; i < 250; i++) {
        for (uint8_t j = i; j < 250; j++) {
            for (uint8_t k = 0; k < 26; k++) {
                if (s[i * 27 + k] == s[j * 27 + k]) {
                    common[ncommon++] = s[j * 27 + k];
                }
            }

            if (ncommon == 25) {
                common[ncommon] = '\0';
                return;

            }

            ncommon = 0;
        }
    }
}


int main() {
    clock_t t = timer_start();
    char input[1024 * 64];
    read_input_file(input, "input.txt");

    int a1 = pt1(input);
    char buf[26];
    pt2(buf, input);

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", a1);
    printf("Part 2: %s\n",  buf);
    printf("Time: %.2fms\n", timer_stop(t));
    return EXIT_SUCCESS;
}
