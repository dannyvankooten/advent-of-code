#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void read_input_file(char *dest, char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    size_t nread = fread(dest, 1, 32*1024, fp);
    dest[nread] = '\0';
    fclose(fp);
}

int validpw(char words[16][32], int nwords) {
    for (int i = 0; i < nwords; i++) {
        for (int j = i+1; j < nwords; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

int compare_chars(const void* a, const void* b)
{
    int arg1 = *(const char*)a;
    int arg2 = *(const char*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int solve(char *s) {
    int count = 0;
    int count_pt2 = 0;
    char words[16][32];

    while (*s != '\0') {
        int nwords = 0;

        while (*s != '\n' && *s != '\0') {
            char *w = words[nwords++];
            while (*s != ' ' && *s != '\n') {
                *w++ = *s++;
            }
            *w = '\0';
            if (*s == ' ') s++;
        }

        count += validpw(words, nwords);

        // for part 2, sort all words alphabetically
        for (int i = 0; i < nwords; i++) {
            qsort(words[i], strlen(words[i]), sizeof(char), compare_chars);
        }
        count_pt2 += validpw(words, nwords);

        if (*s == '\n') s++;
    }

    // return both answers in a single 32-bit int
    return (count_pt2 << 16 ) + count;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[1024 * 64];
    read_input_file(input, "input.txt");

    int answers = solve(input);
    printf("--- Day 4: High-Entropy Passphrases ---\n");
    printf("Part 1: %d\n", answers & 0xFFFF);
    printf("Part 2: %d\n", answers >> 16);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return EXIT_SUCCESS;
}
