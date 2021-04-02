#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

int 
compare(const void *p1, const void *p2) {
    long *a = (long *) p1;
    long *b = (long *) p2;

    if (*a == *b) {
        return 0;
    }

    return (*a < *b) ? -1 : 1;
}

int day10() {
    FILE *f = fopen("10.input", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");

    int64_t adapter_joltages[108];
    size_t adapter_n = 0;
    adapter_joltages[adapter_n++] = 0; // outlet
    char linebuf[BUFSIZ] = {0};
    
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        int64_t n = 0;
        char *s = linebuf;
        while (*s >= '0' && *s <= '9') {
            n = n * 10 + (*s - '0');
            s++;
        }
        adapter_joltages[adapter_n++] = n;
    }
    fclose(f);

    int64_t charging_outlet_joltage = 0; 
    qsort(adapter_joltages, adapter_n, sizeof(int64_t), compare);

    int64_t device_joltage = adapter_joltages[adapter_n-1] + 3;

    int64_t diff[4] = { [0 ... 3] = 0};
    diff[adapter_joltages[0] - charging_outlet_joltage]++;
    for (size_t i=1; i < adapter_n; i++) {
        int64_t d = adapter_joltages[i] - adapter_joltages[i-1];
        diff[d]++;
    }

    // add device difference
    diff[device_joltage - adapter_joltages[adapter_n-1]]++;
    printf("%ld\n", diff[1] * diff[3]);

    // for each adapter, find the number of available options
    int64_t max_diff = 3;
    int64_t ways[adapter_n];
    ways[0] = 1;
    for (size_t i=1; i < adapter_n; i++) {
        ways[i] = 0;

        for (int64_t j=i-1; j >= 0 && (adapter_joltages[i] - adapter_joltages[j]) <= max_diff; j--) {
            ways[i] += ways[j];
        }
    }
    printf("%ld\n", ways[adapter_n-1]);
    return 0;
}
