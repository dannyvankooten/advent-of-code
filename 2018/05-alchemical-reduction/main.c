#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../adventofcode.h"

#define PUZZLE_NAME "Day 5: Alchemical Reduction"

int react(char *input) {
    char *s = input + 1;
    unsigned int remaining_length = strlen(s);
    char o;
    while (*s != 0x0) {
        // determine opposite unit of same type
        if (*s >= 'a' && *s <= 'z') {
            o = *s - 32;
        } else {
            o = *s + 32;
        }

        if (*(s-1) == o) {
           s--;
           remaining_length--;
           memmove(s, s+2, remaining_length);
           *(s+remaining_length) = 0x0;
           continue;
        }

        s++;
        remaining_length--;
    }

    return strlen(input);
}

int pt2(char *input) {
    int shortest = 99999999;
    char input_copy[1024*64];

    for (char c = 'a'; c <= 'z'; c++) {
        strcpy(input_copy, input);
        char *s = input_copy;
        while (*s != 0x0) {
            if (*s == c || *s == c - 32) {
                int length = strlen(s) - 1;
                memmove(s, s+1, length);
                *(s + length) = 0x0;
                continue;
            }
            s++;
        }

        int l = react(input_copy);
        if (l < shortest) {
            shortest = l;
        }
    }

    return shortest;
}

int main() {
    clock_t t = timer_start();
    char input[1024 * 64] = "";
    read_input_file(input, "input.txt");

    int a1 = react(input); 
    int a2 = pt2(input);

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", a1);
    printf("Part 2: %d\n", a2);
    printf("Time: %.2fms\n", timer_stop(t));
    return EXIT_SUCCESS;
}
