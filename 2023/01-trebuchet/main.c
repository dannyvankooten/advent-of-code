#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define BUFSIZE 32*1024

void pt1(char *input, size_t input_len);
void pt2(char *input, size_t input_len);

int main() {


    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        abort();
    }
    char input[BUFSIZE];

    size_t input_len = fread(input, 1, BUFSIZE, fp);
    fclose(fp);

    clock_t start_t, end_t;
    start_t = clock();

    pt1(input, input_len);
    pt2(input, input_len);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("%.2fms\n", total_t);
    return 0;
}

void pt1(char *input, size_t input_len) {
    int sum = 0;
    int ndigits = 0;
    int digits[32] = {0};
    int value = 0;
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];

        if (c == '\n') {
            value = digits[0] * 10 + digits[ndigits-1];
            sum += value;
            ndigits = 0;
            continue;
        }

        if (c >= '0' && c <= '9') {
            digits[ndigits++] = c - '0';
        }
    }

    fprintf(stdout, "part 1: %d\n", sum);
}

typedef struct str {
    uint16_t size;
    char* value;
} str_t;

bool starts_with(char *haystack, str_t needle) {
    return memcmp(haystack, needle.value, needle.size) == 0;
}

void pt2(char *input, size_t input_len) {
    str_t words[] = {
        {3, "one"},
        {3, "two"},
        {5, "three"},
        {4, "four"},
        {4, "five"},
        {3, "six"},
        {5, "seven"},
        {5, "eight"},
        {4, "nine"},
    };
    size_t nwords = 9;
    int sum = 0;
    uint8_t ndigits = 0;
    uint8_t digits[16] = {0};
    int value = 0;
    uint16_t j = 0;

    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];

        if (c >= '0' && c <= '9') {
            digits[ndigits++] = c - '0';
            continue;
        }

        if (c == '\n') {
            value = digits[0] * 10 + digits[ndigits-1];
            sum += value;
            ndigits = 0;
            continue;
        }

        for (j = 0; j < nwords; j++) {
            if (starts_with(input + i, words[j])) {
                digits[ndigits++] = j+1;
                i += words[j].size - 2;
                break;
            }
        }
    }

    fprintf(stdout, "part 2: %d\n", sum);
}
