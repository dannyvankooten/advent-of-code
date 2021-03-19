#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>

int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    unsigned long numbers[1000];
    int numbers_n = 0;
    char linebuf[BUFSIZ] = {0};
    unsigned long long n = 0;
    // unsigned long long invalid_n = 127;
    unsigned long long invalid_n = 104054607;
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
            n = strtoll(linebuf, NULL, 10);
            numbers[numbers_n++] = n;
    }
    fclose(f);
    printf("%d numbers\n", numbers_n);

    // loop through numbers to find contiguous set 
    // that sums to invalid_n (127)
    int range_start = 0;
    int range_end = 0;
    for (int i=0; i < numbers_n-1; i++) {
        unsigned long sum = numbers[i];

        int j = i;
        for (; j < numbers_n-1 && sum < invalid_n; ) {
            sum += numbers[++j];
        }

        if (sum == invalid_n && j > i) {
            range_start = i;
            range_end = j;
            break;
        }
    }

    printf("Found range: %d to %d\n", range_start, range_end);
    unsigned long smallest = numbers[range_start];
    unsigned long largest = numbers[range_start];
    for (int i=range_start; i <= range_end; i++) {
        if (numbers[i] < smallest) {
            smallest = numbers[i];
        } else if(numbers[i] > largest) {
            largest = numbers[i];
        }
    }

    printf("S: %ld\nL: %ld\n", smallest, largest);
    printf("Result: %ld\n", largest + smallest);

    return EXIT_SUCCESS;
}
