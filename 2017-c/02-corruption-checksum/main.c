#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static int compare_ints(const void* a, const void* b)
{
    const int ia = *(const int*)a;
    const int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

int main(void) {
    clock_t start_t, end_t;
    start_t = clock();

    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    char input[32*1024];
    size_t nread = fread(input, 1, 32*1024, fp);
    input[nread] = '\0';
    fclose(fp);

    char *s = input;
    int values[32];
    int checksum = 0;
    int checksum2 = 0;
    while (*s != '\0') {
        int nvalues = 0;

        while (*s != '\n') {
            values[nvalues] = 0;
            while (*s >= '0' && *s <= '9') {
                values[nvalues] = values[nvalues] * 10 + (*s - '0');
                s++;
            }

            nvalues++;
            while (*s == ' ' || *s == '\t') s++;
        }

        // sort, then add diff of lowest and highest to checksum
        qsort(values, nvalues, sizeof(int), compare_ints);
        checksum += values[nvalues-1] - values[0];

        // for pt2, find evely divisible number on each row
        for (int i = 0; i < nvalues; i++) {
            for (int j = i; j < nvalues; j++) {
                if (i == j) continue;

                if (values[j] % values[i] == 0) {
                    checksum2 += values[j] / values[i];

                    // break out of outer loop
                    goto NEXTLINE;
                }
            }
        }

        NEXTLINE:

        // skip \n
        s++;
    }

    printf("--- Day 2: Corruption Checksum ---\n");
    printf("Part 1: %d\n", checksum);
    printf("Part 2: %d\n", checksum2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}
