#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../adventofcode.h"

#define PUZZLE_NAME "Day 10: Knot Hash"


int main() {
    clock_t t = timer_start();
    char input[1024 * 64];
    read_input_file(input, "input.txt");

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", 0);
    printf("Part 2: %d\n", 0);
    printf("Time: %.2fms\n", timer_stop(t));
    return EXIT_SUCCESS;
}
