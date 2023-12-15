#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int parse(const char *s, char needles[64][16], char replacements[64][16],
          char *medicine) {
  int n = 0;
  while (*s != '\0') {
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

int pt1(char needles[64][16], char replacements[64][16], int nreplacements,
        char *const medicine) {
  char *molecules = calloc_or_die(1024 * 512, sizeof(char));
  int nmolecules = 0;
  int pt1 = 0;

  struct replacement {
    int pos;
    char value[16];
  };

  struct replacement *replaced = malloc(128 * sizeof(struct replacement));
  int nreplaced = 0;

  char *m = medicine;
  int pos = 0;
  while (*m != '\0') {
    for (int r = 0; r < nreplacements; r++) {
      if (strncmp(m, needles[r], strlen(needles[r])) != 0) {
        continue;
      }

      char *molecule = molecules + (nmolecules * 512 * sizeof(char));
      strncpy(molecule, medicine, m - medicine);
      strcat(molecule, replacements[r]);
      strcat(molecule, m + strlen(needles[r]));

      replaced[nreplaced].pos = pos;
      strcpy(replaced[nreplaced].value, replacements[r]);
      nreplaced++;

      // is this a unique addition
      int unique = 1;
      for (int j = 0; j < nreplaced; j++) {
        if (replaced[j].pos == pos &&
            strcmp(replaced[j].value, needles[r]) == 0) {
          unique = 0;
          break;
        }
      }
      pt1 += unique;
      nmolecules++;
    }

    m++;
    pos++;
  }

  printf("did %d nreplacements\n", nreplaced);

  free(molecules);
  return pt1;
}

int main() {
  clock_t start_t = clock_time();

  char input[1024 * 32] = "";
  read_input_file(input, 1024 * 32, "input_test.txt");

  char needles[64][16];
  char replacements[64][16];
  char medicine[512];
  int nreplacements = parse(input, needles, replacements, medicine);

  int a1 = pt1(needles, replacements, nreplacements, medicine);

  int pt2 = 0;
  // TODO: Since we have multiple possible replacements
  // We need to split off into a recursion at every replacement with multiple
  // replacement values We can bail a recursion whenever the medicine prefix
  // doesn't match.
  char result[1024 * 16];
  strcpy(result, "e");

  printf("--- Day 19: Medicine for Rudolph ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", pt2);

  printf("Time: %.2fms\n", clock_time_since(start_t));

  return 0;
}