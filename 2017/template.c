#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define PUZZLE_NAME "Day 6: Memory Reallocation"

void read_input_file(char *dest, char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    size_t nread = fread(dest, 1, 64*1024, fp);
    dest[nread] = '\0';
    fclose(fp);
}

int solve(char *s) {
    return 0;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[1024 * 64];
    read_input_file(input, "input.txt");

    int answers = solve(input);
    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", 0);
    printf("Part 2: %d\n", 0);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return EXIT_SUCCESS;
}
