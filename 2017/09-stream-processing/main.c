#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define PUZZLE_NAME "Day 9: Stream Processing"

void read_input_file(char *dest, char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        fprintf(stderr, "error reading %s", file);
        exit(EXIT_FAILURE);
    }
    size_t nread = fread(dest, 1, 64*1024, fp);
    dest[nread] = '\0';
    fclose(fp);
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[1024 * 64];
    read_input_file(input, "input.txt");

    int depth = 0;
    int garbage = 0;
    int score = 0;
    int count = 0;
    char *s = input;
    while (*s != '\0') {

        if (garbage) {
            switch (*s) {
                case '!':
                    s += 2;
                    break;
                case '>':
                    garbage = 0;
                    s++;
                    break;
                default:
                    s++;
                    count++;
                    break;
            }
        } else {
            switch (*s) {
                case '{':
                    depth++;
                    s++;
                    break;
                case '}':
                    score += depth--;
                    s++;
                    break;
                case '<':
                    garbage = 1;
                    s++;
                    break;
                case ',':
                    s++;
                    break;
                default:
                    fprintf(stderr, "Invalid character at position %ld: '%c'\n", s - input, *s);
                    exit(EXIT_FAILURE);
            }
        }
    }

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", score);
    printf("Part 2: %d\n", count);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return EXIT_SUCCESS;
}
