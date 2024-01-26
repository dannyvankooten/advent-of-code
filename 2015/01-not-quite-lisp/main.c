#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  clock_t start_t = clock_time();
  char input[64 * 1024] = "";
  int ninput = (int)read_input_file(input, 64 * 1024, "input.txt");

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
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}
