#include <assert.h>
#include <err.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intmap.h"

struct instruction {
  char mask[36];
  int64_t address;
  int64_t value;
  enum {
    SET_MASK,
    SET_VALUE,
  } type;
};
typedef struct instruction instruction_t;

struct vec {
  int64_t* values;
  size_t size;
  size_t cap;
};
typedef struct vec vec_t;

static void print_binary(int64_t n);

static int64_t
apply_bitmask(const char mask[36], int64_t n) {
  char k;
  int64_t r = 0;

  for (int8_t i = 0; i < 36; i++) {
    // get bit in n at position i
    k = (n >> (35 - i)) & 1;

    // if mask[i] equals 'X', take bit from number
    if ((mask[i] == 'X' && k == 1) || mask[i] == '1') {
      r |= 1UL << (35 - i);
    }
  }

  return r;
}

static int64_t
bin2dec(const char mask[36]) {
  int64_t r = 0;

  for (int8_t i = 0; i < 36; i++) {
    if (mask[i] == '1') {
      r |= 1UL << (35 - i);
    }
  }
  return r;
}
static void
apply_address_mask(vec_t* v, const char mask[36], int64_t n) {
  // generate all possible combinations
  char k;
  size_t m;

  // reset vector
  v->size = 1;
  v->values[0] = 0;

  for (int8_t i = 0; i < 36; i++) {
    switch (mask[i]) {
      case '0':
        k = (n >> (35 - i)) & 1;
        if (k == 1) {
          for (m = 0; m < v->size; m++) {
            v->values[m] |= 1UL << (35 - i);
          }
        }
        break;
      case '1':
        for (m = 0; m < v->size; m++) {
          v->values[m] |= 1UL << (35 - i);
        }
        break;
      case 'X': {
        size_t size_before = v->size;
        // add each variation with i'th bit set to 0
        for (m = 0; m < size_before; m++) {
          v->values[v->size++] = v->values[m];
        }

        // and add each variation with it's bit set to 1
        for (m = 0; m < size_before; m++) {
          v->values[m] |= 1UL << (35 - i);
        }
      } break;
    }
  }
}

static void
print_binary(int64_t n) {
  int8_t c;
  int64_t k;
  for (c = 35; c >= 0; c--) {
    k = n >> c;

    if (k & 1) {
      printf("1");
    } else {
      printf("0");
    }
  }
}

int32_t day14() {
  // assert(apply_bitmask("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X", 11) == 73);
  FILE* f = fopen("inputs/14.txt", "r");
  if (!f) {
    err(EXIT_FAILURE, "error reading input file");
  }
  char linebuf[BUFSIZ] = {0};

  // parse program
  size_t cap = 592;
  size_t size = 0;
  instruction_t* instructions = malloc(cap * sizeof(instruction_t));
  if (!instructions) {
    err(EXIT_FAILURE, "error allocating memory for instructions");
  }
  while (fgets(linebuf, BUFSIZ, f) != NULL) {
    instruction_t* ins = &instructions[size++];
    char* s = linebuf;

    if (strncmp(s, "mask = ", 7) == 0) {
      ins->type = SET_MASK;
      char* mask = ins->mask;
      s += 7;  // skip "mask"
      while (*s != 0 && *(s - 2) != '=') {
        s++;
      }
      while (*s != '\0' && *s != '\n') {
        *mask++ = *s++;
      }
    } else {
      ins->type = SET_VALUE;
      s += 3;  // skip "mem"
      while (*(s - 1) != '[') {
        s++;
      }

      ins->address = 0;
      while (*s >= '0' && *s <= '9') {
        ins->address = (ins->address * 10) + (*s++ - '0');
      }

      while (*s != '=' && *s != '\0') {
        s++;
      }
      while (*s < '0' || *s > '9') {
        s++;
      }

      ins->value = 0;
      while (*s >= '0' && *s <= '9') {
        ins->value = (ins->value * 10) + (*s++ - '0');
      }
    }

    // add instruction to list
    if (size >= cap) {
      cap *= 2;
      instructions = realloc(instructions, cap * sizeof(instruction_t));
      if (!instructions) {
        err(EXIT_FAILURE, "error allocating memory for instructions");
      }
    }
  }
  fclose(f);

  // initialize hashmap for storing memory
  intmap_t* hm = intmap_new(262144);
  // struct hashmap hm = hashmap_new();
  vec_t addresses = {
      .size = 0,
      .cap = 1024,
      .values = malloc(1024 * sizeof(int64_t)),
  };
  if (!addresses.values) {
    err(EXIT_FAILURE, "failed to allocate memory for addresses");
  }

  // walk through instructions
  char mask[36] = {0};
  size_t j;
  int64_t sum = 0;
  for (size_t i = 0; i < size; i++) {
#ifdef STEP
    printf("\n%ld  \tmask=\t\t%.36s\n", i, mask);
#endif

    switch (instructions[i].type) {
      case SET_MASK:
        memcpy(mask, instructions[i].mask, 36);
        break;

      case SET_VALUE: {
#ifdef STEP
        printf("address= \t%ld \t", instructions[i].address);
        print_binary(instructions[i].address);
        printf("\n");
#endif

        apply_address_mask(&addresses, mask, instructions[i].address);
        for (j = 0; j < addresses.size; j++) {
          sum += instructions[i].value;
          sum -= intmap_set(hm, addresses.values[j], instructions[i].value);
        }
      } break;
    }
  }

  printf("%ld\n", sum);
  assert(sum == 4173715962894);

  free(instructions);
  free(addresses.values);
  intmap_free(hm);
  return 0;
}
