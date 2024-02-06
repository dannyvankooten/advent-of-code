#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void read_input_file(char *dest, const char *file) {
  FILE *fp = fopen(file, "r");
  if (!fp) {
    fprintf(stderr, "error reading input.txt");
    exit(EXIT_FAILURE);
  }
  size_t nread = fread(dest, 1, 64 * 1024, fp);
  dest[nread] = '\0';
  fclose(fp);
}

unsigned int validpw(char words[16][32], const unsigned int nwords) {
  for (unsigned int i = 0; i < nwords; i++) {
    for (unsigned int j = i + 1; j < nwords; j++) {
      if (strcmp(words[i], words[j]) == 0) {
        return 0;
      }
    }
  }
  return 1;
}

static int compare_chars(const void *a, const void *b) {
  const char ca = *(const char *)a;
  const char cb = *(const char *)b;
  return (ca > cb) - (ca < cb);
}

static unsigned int solve(const char *s) {
  unsigned int count = 0;
  unsigned int count_pt2 = 0;
  char words[16][32];

  // go over every line
  while (*s != '\0') {
    unsigned int nwords = 0;


    // go over every word in line
    while (*s != '\n' && *s != '\0' && nwords < 16) {
      char *w = words[nwords++];
      unsigned len = 0;

      while (*s != ' ' && *s != '\n' && len < 32) {
        w[len++] = *s++;
      }
      w[len] = '\0';

      if (*s == ' ') {
        s++;
      }
    }

    count += validpw(words, nwords);

    // for part 2, sort all words alphabetically
    for (unsigned int i = 0; i < nwords; i++) {
      qsort(words[i], strlen(words[i]), sizeof(char), compare_chars);
    }
    count_pt2 += validpw(words, nwords);

    s++;
  }

  // return both answers in a single 32-bit int
  return (count_pt2 << 16) + count;
}

int main(void) {
  clock_t start_t, end_t;
  start_t = clock();

  char input[1024 * 64];
  read_input_file(input, "input.txt");

  unsigned int answers = solve(input);
  printf("--- Day 4: High-Entropy Passphrases ---\n");
  printf("Part 1: %d\n", answers & 0xFFFF);
  printf("Part 2: %d\n", answers >> 16);

  end_t = clock();
  double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
  printf("Time: %.2fms\n", total_t);
  return EXIT_SUCCESS;
}
