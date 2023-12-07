#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int pt1() {

    for (int h = 1; ; h++) {
        int presents = 0;

        for (int e = 1; ; e++) {
            if (e % h == 0) {
                presents += e*10;
            }

            if (presents > 34000000) {
                return h;
            }
        }
    }

    return -1;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();




    int a1 = pt1();



    int a2 = 0;

    printf("--- Day 20: Infinite Elves and Infinite Houses ---\n");
    printf("Part 1: %d\n", a1);
    printf("Part 2: %d\n", a2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}