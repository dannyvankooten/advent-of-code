#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int str_is_nice(char *s) {
  int charcount[26] = {0};
  int repeats = 0;
  while (*s != '\n' && *s != '\0') {
    charcount[*s - 'a']++;

    if (*s == *(s + 1)) {
      repeats++;
    }

    if (memcmp(s, "ab", 2) == 0 || memcmp(s, "cd", 2) == 0 ||
        memcmp(s, "pq", 2) == 0 || memcmp(s, "xy", 2) == 0) {
      return 0;
    }

    s++;
  }

  int vowelcount = 0;
  char *vowels = "aeiou";
  int nvowels = 5;
  for (int i = 0; i < nvowels; i++) {
    vowelcount += charcount[vowels[i] - 'a'];
  }

  return repeats > 0 && vowelcount >= 3 ? 1 : 0;
}

int str_is_nice_v2(char *s) {
  int r1 = 0;
  int r2 = 0;
  while (*s != '\n' && *s != '\0') {

    // check for rule 1
    char *s2 = s;
    if (r1 == 0 && *(s + 1) != '\n' && *(s + 2) != '\n' && *(s + 2) != '\n' &&
        *(s + 2) != '\0') {
      s2 += 2;
      while (r1 == 0 && *s2 != '\n' && *s2 != '\0') {
        if (memcmp(s, s2, 2) == 0) {
          r1 = 1;
        }

        s2++;
      }
    }

    // check for rule 2
    if (*(s + 1) != '\n' && *(s + 2) == *s) {
      r2 = 1;
    }

    s++;
  }

  return r1 == 1 && r2 == 1;
}

int main() {
  clock_t start_t, end_t;
  start_t = clock();

  FILE *fp = fopen("input.txt", "r");
  if (fp == NULL) {
    fprintf(stderr, "error reading input.txt");
    exit(EXIT_FAILURE);
  }
  char input[32 * 1024];
  size_t nread = fread(input, 1, 32 * 1024, fp);
  input[nread] = '\0';
  fclose(fp);

  char *s = input;
  int nice_v1 = 0;
  int nice_v2 = 0;
  while (*s != '\0') {
    if (str_is_nice(s) == 1) {
      nice_v1++;
    }

    if (str_is_nice_v2(s) == 1) {
      nice_v2++;
    }

    while (*s != '\n' && *s != '\0')
      s++;
    if (*s == '\n')
      s++;
  }

  printf("--- Day 5: Doesn't He Have Intern-Elves For This? ---\n");
  printf("Part 1: %d\n", nice_v1);
  printf("Part 2: %d\n", nice_v2);

  end_t = clock();
  double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
  printf("Time: %.2f ms\n", total_t);
  return 0;
}
