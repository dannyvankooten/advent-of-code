#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>

int day1();
int day2();
int day3();
int day4();
int day5();
int day6();
int day7();
int day8();
int day9();
int day10();
int day11();
int day12();
int day13();
int day14();
int day15();
int day16();
int day17();
int day18();
int day19();
int day20();
int day21();
int day22();
int day23();
int day24();
int day25();

int (*functions[])() = {
    day1,
    day2,
    day3,
    day4,
    day5,
    day6,
    day7,
    day8,
    day9,
    day10,
    day11,
    day12,
    day13,
    day14,
    day15,
    day16,
    day17,
    day18,
    day19,
    day20,
    day21,
    day22,
    day23,
    day24,
    day25,
};

int main() {
    double total_time = 0;

    for (int32_t d=1; d <= 25; d++) {
        char path[256];
        sprintf(path, "../%02d", d);
        chdir(path);
        printf("Day %d: ", d);

        double best_elapsed_time = 10;
        for (int32_t i=0; i < 1; i++) {
            clock_t start = clock();
            functions[d-1]();
            clock_t end = clock();
            double elapsed_time = ((double) end - start) / CLOCKS_PER_SEC;
            if (elapsed_time < best_elapsed_time) {
                best_elapsed_time = elapsed_time;
            }
        }
        printf("(%.4fs)\n", best_elapsed_time);
        total_time += best_elapsed_time;
    }

    printf("Total: %.0fms\n", total_time * 1000);
}