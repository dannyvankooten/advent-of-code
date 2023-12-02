#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define BUFSIZE 32*1024

void solve(const char *input, size_t input_len);

int main() {
    char input[BUFSIZE];
    FILE *fp = fopen("input.txt", "r");
    size_t input_len = fread(input, 1, BUFSIZE, fp);
    fclose(fp);

    clock_t start_t, end_t;
    start_t = clock();

    solve(input, input_len);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("%.2fms\n", total_t);
    return 0;
}

typedef struct str {
    uint16_t size;
    char* value;
} str_t;

static inline
bool str_starts_with(const char *haystack, str_t needle) {
    return memcmp(haystack, needle.value, needle.size) == 0;
}

void solve(const char *input, size_t input_len) {
    uint16_t sum = 0;
    uint16_t sum2 = 0;
    uint8_t digits[32] = {0};
    uint8_t digits2[32] = {0};
    uint8_t ndigits = 0;
    uint8_t ndigits2 = 0;

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
    uint8_t nwords = 9;

    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];

        if (c == '\n') {
            // sum pt1
            sum += digits[0] * 10 + digits[ndigits-1];
            ndigits = 0;

            // sum pt2
            sum2 += digits2[0] * 10 + digits2[ndigits2-1];;
            ndigits2 = 0;
            continue;
        }

        if (c >= '0' && c <= '9') {
            digits[ndigits++] = c - '0';
            digits2[ndigits2++] = c - '0';
            continue;
        }

         for (uint8_t j = 0; j < nwords; j++) {
            if (str_starts_with(input + i, words[j])) {
                digits2[ndigits2++] = j+1;
                i += words[j].size - 2;
                break;
            }
        }
    }

    fprintf(stdout, "part 1: %d\n", sum);
    fprintf(stdout, "part 2: %d\n", sum2);
}

