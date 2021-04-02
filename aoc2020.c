#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
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
    day1,  day2,  day3,  day4,  day5,  day6,  day7,  day8,  day9,
    day10, day11, day12, day13, day14, day15, day16, day17, day18,
    day19, day20, day21, day22, day23, day24, day25,
};

int64_t run_day(int8_t day, int8_t samples) {
  struct timeval time_start, time_end, time_diff;
  int64_t best_elapsed_time = 1000000;

  for (int8_t i = 0; i < samples; i++) {
    gettimeofday(&time_start, NULL);
    functions[day - 1]();
    gettimeofday(&time_end, NULL);
    timersub(&time_end, &time_start, &time_diff);
    int64_t elapsed_time = (int64_t)time_diff.tv_usec;
    if (elapsed_time < best_elapsed_time) {
      best_elapsed_time = elapsed_time;
    }
  }

  return best_elapsed_time;
}

int main(int argc, char** argv) {
  int8_t n_samples = 1;
  int8_t day = 0;

  // parse args
  for (int8_t i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--bench") == 0) {
      n_samples = 10;
    } else if (isdigit(*argv[i])) {
      while (*argv[i] != '\0') {
        day = day * 10 + (*argv[i] - '0');
        argv[i]++;
      }
    }
  }

  if (day > 0) {
    int64_t elapsed_time = run_day(day, n_samples);
    printf("Ran in %8ld μs\n", elapsed_time);
    return 0;
  }

  int64_t total_time = 0;
  for (int8_t d = 1; d <= 25; d++) {
    printf("Day %d:\t", d);
    freopen("/dev/null", "w", stdout);
    int64_t elapsed_time = run_day(d, n_samples);
    freopen("/dev/tty", "w", stdout);
    printf("%8ld μs\n", elapsed_time);
    total_time += elapsed_time;
  }

  printf("Total:\t%8ld μs (%.2f s)\n", total_time,
         (double)total_time / 1000000.0);
  return 0;
}