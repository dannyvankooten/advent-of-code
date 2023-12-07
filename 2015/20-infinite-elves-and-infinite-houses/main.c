#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int pt1(int *houses, int nhouses, int target, int elfvalue, int elflimit) {
    int elf, house, j;

    if (elflimit > 0) {
        for (elf = 1; elf < nhouses; elf++) {
            for (house = elf, j = 0; house < nhouses && j < elflimit; house += elf, j++) {
                houses[house] += elf * elfvalue;
            }
        }
    } else {
        // simpler loop without elf limit
        for (elf = 1; elf < nhouses; elf++) {
            for (house = elf; house < nhouses; house += elf) {
                houses[house] += elf * elfvalue;
            }
        }
    }

    for (int i = 1; i < nhouses; i++) {
        if (houses[i] > target) {
            return i;
        }
    }

    return -1;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    int target = 34000000;
    int nhouses = target / 10;
    int *houses = (int *) malloc(sizeof(int) * nhouses);
    memset(houses, 0, nhouses * sizeof(int));

    int a1 = pt1(houses, nhouses, target, 10, -1);

    memset(houses, 0, nhouses * sizeof(int));
    nhouses = target / 11;
    int a2 = pt1(houses, nhouses, target, 11, 50);

    printf("--- Day 20: Infinite Elves and Infinite Houses ---\n");
    printf("Part 1: %d\n", a1);
    printf("Part 2: %d\n", a2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);

    free(houses);
    return 0;
}