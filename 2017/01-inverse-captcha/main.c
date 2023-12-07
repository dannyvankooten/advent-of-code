#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int solve(const char *input, size_t size, int offset) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        int next = (i + offset) % (size-1);
        if (input[i] == input[next]) {
            sum += input[i] - '0';
        }
    }

    return sum;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        abort();
    }
    char input[32*1024];
    size_t nread = fread(input, 1, 32*1024, fp);
    input[nread] = '\0';
    fclose(fp);

    printf("--- Day 1: Inverse Captcha ---\n");
    printf("Part 1: %d\n", solve(input, nread, 1));
    printf("Part 2: %d\n", solve(input, nread, (nread-1)/2));

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}
