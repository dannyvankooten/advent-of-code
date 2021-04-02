#include <bits/stdint-intn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>
#include <sys/time.h>

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

int64_t 
run_day(int8_t d) {
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);
    functions[d-1]();
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    return (int64_t) tval_result.tv_usec / 1000; 
}

int main(int argc, char **argv) {
    if (argc > 1) {
        int32_t n = 0;
        char *s = argv[1];
        while (*s != '\0') {
            n = n * 10 + (*s - '0');
            s++;
        }
        run_day(n);
        run_day(n);
        return EXIT_SUCCESS;
    }

    int64_t total_time = 0;

    for (int32_t d=1; d <= 25; d++) {
        printf("Day %d:\t", d);
        freopen("/dev/null","w",stdout);
        int64_t best_elapsed_time = 1000000;
        for (int32_t i=0; i < 10; i++) {
            int64_t elapsed_time = run_day(d);
            if (elapsed_time < best_elapsed_time) {
                best_elapsed_time = elapsed_time;
            }
        }
        freopen("/dev/tty","w",stdout);
        printf("%4ld ms\n", best_elapsed_time);
        total_time += best_elapsed_time;
    }

    printf("Total:\t%4ld ms\n", total_time);
}