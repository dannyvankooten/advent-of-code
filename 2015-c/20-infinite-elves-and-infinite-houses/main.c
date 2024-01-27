#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int pt1(int *houses, int nhouses, int target, int elfvalue, int elflimit) {

  if (elflimit > 0) {
    for (int elf = 1; elf < nhouses; elf++) {
      for (int house = elf, j = 0; house < nhouses && j < elflimit;
           house += elf, j++) {
        houses[house] += elf * elfvalue;
      }
    }
  } else {
    // simpler loop without elf limit
    for (int elf = 1; elf < nhouses; elf++) {
      for (int house = elf; house < nhouses; house += elf) {
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

int main(void) {
  clock_t start_t = clock_time();

  char input[64];
  read_input_file(input, 64, "input.txt");
  int target;
  parse_int(&target, input);

  int nhouses = target / 10;
  int *houses = malloc(sizeof(int) * (size_t)nhouses);
  if (houses == NULL) {
    perror("error allocating memory");
    exit(EXIT_FAILURE);
  }
  memset(houses, 0, (size_t)nhouses * sizeof(int));

  int a1 = pt1(houses, nhouses, target, 10, -1);

  memset(houses, 0, (size_t)nhouses * sizeof(int));
  nhouses = target / 11;
  int a2 = pt1(houses, nhouses, target, 11, 50);

  printf("--- Day 20: Infinite Elves and Infinite Houses ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);

  printf("Time: %.2f ms\n", clock_time_since(start_t));

  free(houses);
  return 0;
}
