#include "../adventofcode.h"
#include "../hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int parse(const char *s, char needles[][16], char replacements[][16],
          char *medicine) {

  int n = 0;
  while (*s != '\0') {
    // parse needles => replacements until double newline
    while (*s != '\n') {
      s = parse_ident(needles[n], s);
      s = skip(" => ", s);
      s = parse_ident(replacements[n], s);
      s = skip_optional('\n', s);
      n++;
    }

    // parse medicine
    s = skip("\n", s);
    s = parse_ident(medicine, s);
    s = skip_optional('\n', s);
  }

  return n;
}

int pt1(const char needles[][16], char replacements[][16], int n,
        char *medicine) {
  hashmap_t hm = hashmap_new(1024);
  int nmolecules = 0;
  char buf[1024];
  size_t len = strlen(medicine);
  for (size_t pos = 0; pos < len; pos++) {
    for (int i = 0; i < n; i++) {
      const char *needle = needles[i];

      // check if current position matches needle
      size_t needle_length = strlen(needle);
      if (strncmp(&medicine[pos], needle, needle_length) != 0) {
        continue;
      }

      char *replacement = replacements[i];
      char *m = buf;
      size_t suffix_length = len - pos - needle_length;
      size_t replacement_length = strlen(replacement);

      memcpy(m, medicine, pos);
      m += pos;

      memcpy(m, replacement, replacement_length);
      m += replacement_length;

      memcpy(m, &medicine[pos + needle_length], suffix_length);
      m += suffix_length;
      *m = 0x0;

      int *seen =
          hashmap_entry(&hm, (unsigned char *)buf, (size_t)m - (size_t)buf);
      if (*seen != 1) {
        *seen = 1;
        nmolecules++;
      }
    }
  }

  hashmap_free(&hm);
  return nmolecules;
}

// for part 2 a recursive solution isn't feasible
// but a greedy algorithm starting from "e" doesn't work
// working backwards doesn't work on the sample input "HOHOHO" but it
// does on my actual input...
int pt2(char needles[][16], char replacements[][16], int n, char *medicine) {
  char buf[1024];
  int steps = 0;

  while (strcmp(medicine, "e") != 0) {
    for (int i = 0; i < n; i++) {
      const char *needle = needles[i];
      const char *replacement = replacements[i];

      char *pos = strstr(medicine, replacement);
      if (pos != NULL) {
        strcpy(buf, pos + strlen(replacement));
        strcpy(pos, needle);
        strcat(pos, buf);
        steps += 1;
      }
    }
  }

  return steps;
}

int main(void) {
  clock_t start_t = clock_time();

  char input[1024 * 32] = "";
  read_input_file(input, 1024 * 32, "input.txt");

  char needles[64][16];
  char replacements[64][16];
  char medicine[1024];
  int nreplacements = parse(input, needles, replacements, medicine);

  int a1 = pt1(needles, replacements, nreplacements, medicine);
  int a2 = pt2(needles, replacements, nreplacements, medicine);

  printf("--- Day 19: Medicine for Rudolph ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}
