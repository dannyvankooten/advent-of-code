#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const char* input =
    "1002576\n"
    "13,x,x,x,x,x,x,37,x,x,x,x,x,449,x,29,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,"
    "19,x,"
    "x,x,23,x,x,x,x,x,x,x,773,x,x,x,x,x,x,x,x,x,41,x,x,x,x,x,x,17";

int day13(void) {
  const char* s = input;
  int ready_timestamp = 0;
  while (*s != '\n') {
    ready_timestamp = (ready_timestamp * 10) + (*s - '0');
    s++;
  }

  s++;  // Skip '\n'

  // parse bus schedules from 2nd line
  int buses[64] = {0};
  int nbuses = 0;
  while (*s != '\0') {
    if (*s == 'x') {
      buses[nbuses++] = 1;
      s++;
    } else {
      int n = 0;
      while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
      }
      buses[nbuses++] = n;
    }

    if (*s == ',')
      s++;  // skip comma
  }

  // // find idx of highest bus ID (so we can use that to increment search
  // step) int highest_bus_id_idx = 0; for (int i=0; i < nbuses; i++) {
  //     if (buses[i] > buses[highest_bus_id_idx]) {
  //         highest_bus_id_idx = i;
  //     }
  // }

  // int t2;
  // for (unsigned long long t=buses[highest_bus_id_idx]-highest_bus_id_idx; t
  // < ULLONG_MAX - buses[highest_bus_id_idx] - 1; t +=
  // buses[highest_bus_id_idx])
  // {
  //     for (i =0, t2=t; i < nbuses && (buses[i] == 1 || t2 % buses[i] == 0);
  //     i++, t2++);

  //     if (i >= nbuses - 1) {
  //         printf("t = %lld\n", t);
  //         break;
  //     }
  // }

  // Above solution takes several minutes... Had to look up Chinese remainder
  // Theorem. https://en.wikipedia.org/wiki/Chinese_remainder_theorem
  int64_t t = 0;
  int64_t step = buses[0];
  for (int b = 0; b < nbuses; b++) {
    while (1) {
      if ((t + b) % buses[b] == 0) {
        step *= (int64_t) buses[b];
        break;
      }

      t += step;
    }
  }

  // Part 2
  printf("%ld\n", t);
  assert(t == 415579909629976);
  return EXIT_SUCCESS;
}
