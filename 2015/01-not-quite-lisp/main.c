#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    FILE *fp = fopen("input.txt", "r");
    char input[64*1024];
    size_t ninput = fread(input, 1, 64*1024, fp);
    fclose(fp);
    input[ninput] = 0;

    int v = 0;
    int pos = -1;
    for (int i = 0; i < ninput; i++) {
        if (input[i] == '(') {
            v++;
        } else if (input[i] == ')') {
            v--;
        }

        if (pos == -1 && v == -1) {
            pos = i + 1;
        }
    }

    fprintf(stdout, "--- Day 1: Not Quite Lisp ---\n");
    fprintf(stdout, "Part 1: %d\n", v);
    fprintf(stdout, "Part 2: %d\n", pos);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}
