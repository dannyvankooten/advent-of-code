#include "../adventofcode.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static unsigned int pt1(const unsigned nhouses, unsigned int houses[static nhouses], const unsigned int target, const unsigned int elfvalue, const unsigned int elflimit) {

  if (elflimit > 0) {
    for (unsigned elf = 1; elf < nhouses; elf++) {
      for (unsigned house = elf, j = 0; house < nhouses && j < elflimit;
           house += elf, j++) {
        houses[house] += elf * elfvalue;
      }
    }
  } else {
    // simpler loop without elf limit
    for (unsigned elf = 1; elf < nhouses; elf++) {
      for (unsigned house = elf; house < nhouses; house += elf) {
        houses[house] += elf * elfvalue;
      }
    }
  }

  for (unsigned i = 1; i < nhouses; i++) {
    if (houses[i] > target) {
      return i;
    }
  }

  return 0;
}

int main(void) {
  clock_t start_t = clock_time();

  char input[64];
  read_input_file(input, 64, "input.txt");
  unsigned int target = (unsigned int) atoi(input);

  unsigned int nhouses = target / 10;
  unsigned int *houses = malloc(sizeof(int) * (size_t)nhouses);
  assert(houses != NULL);
  memset(houses, 0, nhouses * sizeof(int));

  unsigned int a1 = pt1(nhouses, houses,  target, 10, 0);

  memset(houses, 0, nhouses * sizeof(int));
  nhouses = target / 11;
  unsigned int a2 = pt1(nhouses, houses, target, 11, 50);

  printf("--- Day 20: Infinite Elves and Infinite Houses ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  free(houses);
  return 0;
}
