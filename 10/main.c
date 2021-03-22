#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

#define input_file "input.txt"

int compare(const void *p1, const void *p2) {
    long *a = (long *) p1;
    long *b = (long *) p2;

    if (*a == *b) {
        return 0;
    }

    return (*a < *b) ? -1 : 1;
}

int main() {
    printf("Input file: %s\n", input_file);
    FILE *f = fopen(input_file, "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");

    long adapter_joltages[108];
    int adapter_n = 0;
    adapter_joltages[adapter_n++] = 0; // outlet
    char linebuf[BUFSIZ] = {0};
    unsigned long n = 0;
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        n = strtol(linebuf, NULL, 10);
        adapter_joltages[adapter_n++] = n;
    }
    fclose(f);

    printf("Read %d adapter joltages.\n", adapter_n);
    int charging_outlet_joltage = 0; 
    qsort(adapter_joltages, adapter_n, sizeof(unsigned long), compare);

    long device_joltage = adapter_joltages[adapter_n-1] + 3;
    printf("Device joltage: %ld\n", device_joltage);

    int diff[4] = { [0 ... 3] = 0};
    diff[adapter_joltages[0] - charging_outlet_joltage]++;
    for (int i=1; i < adapter_n; i++) {
        int d = adapter_joltages[i] - adapter_joltages[i-1];
        diff[d]++;
    }

    // add device difference
    diff[device_joltage - adapter_joltages[adapter_n-1]]++;
    printf("Answer: %d\n", diff[1] * diff[3]);

    // for each adapter, find the number of available options
    int max_diff = 3;
    long long ways[adapter_n];
    ways[0] = 1;
    for (int i=1; i < adapter_n; i++) {
        ways[i] = 0;

        for (int j=i-1; j >= 0 && (adapter_joltages[i] - adapter_joltages[j]) <= max_diff; j--) {
            ways[i] += ways[j];
        }
    }
    printf("Answer (part 2): %lld\n", ways[adapter_n-1]);

}