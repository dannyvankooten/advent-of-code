#include "inputs/04.h"
#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// https://adventofcode.com/2020/day/4#part2

static bool is_passport_valid(const char passport[]) {
  // here we only have to compare the 7 first elements, because CID is ignored
  static const char valid_1[] = {1, 1, 1, 1, 1, 1, 1, 1};
  return (memcmp(passport, valid_1, 7 * sizeof(char)) == 0);
}

static bool is_valid_hgt(const char *v) {
  int32_t n = 0;
  int8_t i = 0;

  while (*v >= '0' && *v <= '9') {
    n = n * 10 + (*v - '0');
    i++;
    v++;
  }
  if (i == 0 || i > 3) {
    return false;
  }

  if (strcmp(v, "cm") == 0) {
    return n >= 150 && n <= 193;
  }

  if (strcmp(v, "in") == 0) {
    return n >= 59 && n <= 76;
  }

  return false;
}

static bool is_valid_hcl(const char *v) {
  if (v[0] != '#') {
    return false;
  }

  for (int8_t i = 1; i < 7; i++) {
    if (v[i] == '\0') {
      return false;
    }

    if (!('a' <= v[i] && v[i] <= 'f') && !('0' <= v[i] && v[i] <= '9')) {
      return false;
    }
  }

  if (v[7] != '\0') {
    return false;
  }

  return true;
}

static bool is_valid_ecl(const char *v) {
  static const char *valid_values[] = {"amb", "blu", "brn", "gry",
                                       "grn", "hzl", "oth"};

  for (int8_t i = 0; i < 7; i++) {
    if (strcmp(v, valid_values[i]) == 0) {
      return true;
    }
  }

  return false;
}

static bool is_valid_pid(const char *v) {
  for (int8_t i = 0; i < 9; i++) {
    if (v[i] < '0' || v[i] > '9' || v[i] == '\0') {
      return false;
    }
  }

  if (v[9] != '\0') {
    return false;
  }

  return true;
}

enum field_key { BYR = 0, IYR, EYR, HGT, HCL, ECL, PID, CID };

int day4() {
  char fields[8] = {0};
  char key[32];
  char value[32];
  size_t i;
  int32_t valid_pp_count = 0;
  int32_t num;
  char *v;
  const unsigned char *s = input;

  while (*s != '\0') {
    // blank line, new passport
    if (*s == '\n') {
      if (is_passport_valid(fields)) {
        valid_pp_count++;
      }
      memset(fields, 0, 8 * sizeof(char));
      s++;
      continue;
    }

    while (*s != '\n' && *s != '\0') {
      num = 0;
      v = value;

      // parse key
      for (i = 0; *s != ':'; i++) {
        key[i] = *s++;
      }
      key[i] = '\0';

      // skip :
      s++;

      // parse value (or skip for now)
      for (i = 0; *s != ' ' && *s != '\n' && *s != '\0'; i++) {
        value[i] = *s++;
      }
      value[i] = '\0';

      // skip spaces
      while (*s == ' ') {
        s++;
      }

      if (strcmp(key, "byr") == 0) {
        while (isdigit(*v)) {
          num = (num * 10) + (*v++ - '0');
        }

        if (num >= 1920 && num <= 2002) {
          fields[BYR] = 1;
        }
      } else if (strcmp(key, "iyr") == 0) {
        while (isdigit(*v)) {
          num = (num * 10) + (*v++ - '0');
        }
        if (num >= 2010 && num <= 2020) {
          fields[IYR] = 1;
        }
      } else if (strcmp(key, "eyr") == 0) {
        while (isdigit(*v)) {
          num = (num * 10) + (*v++ - '0');
        }
        if (num >= 2020 && num <= 2030) {
          fields[EYR] = 1;
        }
      } else if (strcmp(key, "ecl") == 0) {
        fields[ECL] = is_valid_ecl(value);
      } else if (strcmp(key, "hgt") == 0) {
        fields[HGT] = is_valid_hgt(value);
      } else if (strcmp(key, "hcl") == 0) {
        fields[HCL] = is_valid_hcl(value);
      }
      if (strcmp(key, "pid") == 0) {
        fields[PID] = is_valid_pid(value);
      }

      if (*s == '\n')
        s++;
    }
  }

  // last line
  if (is_passport_valid(fields)) {
    valid_pp_count++;
  }

  printf("%d\n", valid_pp_count);
  assert(valid_pp_count == 150);
  return 0;
}
