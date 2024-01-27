#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    day1,  day2,  day3,  day4,  day5,  day6,  day7,  day8,  day9,
    day10, day11, day12, day13, day14, day15, day16, day17, day18,
    day19, day20, day21, day22, day23, day24, day25,
};

clock_t timer_start() {
    return clock();
}

double timer_stop(clock_t start_t) {
    clock_t end_t = clock();
    return (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
}

double run_day(int day, int samples) {
  clock_t timer;
  double best_elapsed_time = 1 << 30;

  for (int i = 0; i < samples; i++) {
    timer = timer_start();
    functions[day - 1]();
    double elapsed_time = timer_stop(timer);
    if (elapsed_time < best_elapsed_time) {
      best_elapsed_time = elapsed_time;
    }
  }

  return best_elapsed_time;
}

static void usage(const char *argv0) {
	printf("Usage: %s [1-25] [options]\n", argv0);
	puts(
			"Options:\n"
			"\t-h, --help       Show this help text\n"
			"\t-b, --bench      Run 10 times, take best time\n");
}

int main(int argc, char** argv) {
  int n_samples = 1;
  int day = 0;

  // parse CLI args
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
			usage(argv[0]);
			return EXIT_SUCCESS;
		} else if (strcmp(argv[i], "--bench") == 0 || strcmp(argv[i], "-b") == 0) {
      n_samples = 100;
    } else if (isdigit(*argv[i])) {
      while (isdigit(*argv[i])) {
        day = day * 10 + (*argv[i] - '0');
        argv[i]++;
      }
    }
  }

  // run a single day
  if (day > 0) {
    double elapsed_time = run_day(day, n_samples);
    printf("Ran in %8.2f ms\n", elapsed_time);
    return 0;
  }

  // run all days consecutively
  double total_time = 0;
  double times[25] = {0};
  for (int d = 1; d <= 25; d++) {
    double elapsed_time = run_day(d, n_samples);
    times[d-1] = elapsed_time;
    total_time += elapsed_time;
  }

  // print timings per day
  for (int i=0; i < 25; i++) {
    printf("Day %d:\t", i+1);
    printf("%8.2f ms\n", times[i]);
  }

  // print total time
  printf("Total:\t%8.2f ms\n", total_time);
  return 0;
}
