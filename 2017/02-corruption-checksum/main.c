#include <stdlib.h>
#include <stdio.h>

int compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}


int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        abort();
    }
    char input[32*1024];
    size_t nread = fread(input, 1, 32*1024, fp);
    input[nread] = '\0';

    char *s = input;
    int *values = malloc(32 * sizeof(int));
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
                }
            }
        }

        // skip \n
        s++;
    }

    printf("%d\n", checksum);
    printf("%d\n", checksum2);
    free(values);
    return 0;
}
