#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputs/08.h"

enum instruction_types { NOOP, ACC, JUMP };

struct Instruction {
  int32_t value;
  enum instruction_types type;
  char sign;
};

struct Instructions {
  struct Instruction* values;
  size_t cap;
  size_t size;
};

const unsigned char *
 parse_instruction_line(const unsigned char* line, struct Instruction *ins) {
  char buf[32];
  int32_t i = 0;
  while (*line != ' ') {
    buf[i++] = *line++;
  };
  buf[i] = '\0';

  if (strcmp(buf, "nop") == 0) {
    ins->type = NOOP;
  } else if (strcmp(buf, "acc") == 0) {
    ins->type = ACC;
  } else if (strcmp(buf, "jmp") == 0) {
    ins->type = JUMP;
  } else {
    err(EXIT_FAILURE, "Invalid op type");
  }

  // skip spaces
  while (*line == ' ')
    line++;

  // read sign
  ins->sign = *line == '+' ? 1 : -1;
  line++;

  // skip spaces
  while (*line == ' ')
    line++;

  // read value
  int32_t v = 0;
  while (*line != '\n' && *line != '\0') {
    v = v * 10 + (*line - '0');
    line++;
  }
  ins->value = v * ins->sign;
  return line;
}

int day8() {
  const unsigned char *s = input;
  struct Instructions ins = {
      .cap = 64,
      .size = 0,
      .values = malloc(64 * sizeof(struct Instruction)),
  };
  if (!ins.values) {
    err(EXIT_FAILURE, "Error allocating memory for instruction values");
  }

  // Parse input file
  while (*s != '\0') {
    struct Instruction* i = &ins.values[ins.size++];
    s = parse_instruction_line(s, i);
    if (ins.size == ins.cap) {
      ins.cap *= 2;
      ins.values = realloc(ins.values, ins.cap * sizeof(struct Instruction));
      if (!ins.values) {
        err(EXIT_FAILURE, "Error allocating memory for instruction values");
      }
    }

    if (*s == '\n') s++;
  }

  int32_t* seen = malloc(ins.size * sizeof(int32_t));
  if (!seen) {
    err(EXIT_FAILURE, "error allocating memory for seen array");
  }
  struct Instruction i;

  for (size_t c = 0; c < ins.size; c++) {
    int32_t acc = 0;
    size_t ip;
    memset(seen, 0, ins.size * sizeof(0));

    for (ip = 0; ip < ins.size;) {
      i = ins.values[ip];

      // break if we've seen this instruction already
      if (seen[ip] == 1) {
        break;
      }
      seen[ip] = 1;

      enum instruction_types type = i.type;
      if (c == ip) {
        if (type == NOOP && i.value != 0) {
          type = JUMP;
        } else if (type == JUMP) {
          type = NOOP;
        }
      }

      switch (type) {
        case NOOP:
          // Do nothing special
          break;

        case ACC:
          acc += i.value;
          break;

        case JUMP:
          ip += i.value;
          continue;  // continue without increment instruction pointer
          break;
      }

      ip++;
    }

    // Print result if we made it to end of program
    if (ip == ins.size) {
      printf("%d\n", acc);
      assert(acc == 2092);
    }
  }

  free(ins.values);
  free(seen);
  return 0;
}
