#include "adventofcode.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 19: Go With The Flow"

enum OpCode {
  OP_ADD_R,
  OP_ADD_I,
  OP_MUL_R,
  OP_MUL_I,
  OP_BAN_R,
  OP_BAN_I,
  OP_BOR_R,
  OP_BOR_I,
  OP_SET_R,
  OP_SET_I,
  OP_GT_IR,
  OP_GT_RI,
  OP_GT_RR,
  OP_EQ_IR,
  OP_EQ_RI,
  OP_EQ_RR,
};

struct Op {
  enum OpCode code;
  int a;
  int b;
  int c;
};

static const char *opcode_names[] = {
  "addr",
  "addi",
  "mulr",
  "muli",
  "banr",
  "bani",
  "borr",
  "bori",
  "setr",
  "seti",
  "gtir",
  "gtri",
  "gtrr",
  "eqir",
  "eqri",
  "eqrr",
};

static struct Op parse_op(const char s[static 1024]) {
  struct Op op;

  for (unsigned i = 0 ; i < sizeof(opcode_names) / sizeof(*opcode_names); i++) {
    if (memcmp(s, opcode_names[i], 4) == 0) {
      op.code = (enum OpCode) i;
      break;
    }
  }
  s += 5;

  assert(isdigit(*s));
  op.a = atoi(s);
  while (!isspace(*s)) {
    s++;
  }

  s++;
  assert(isdigit(*s));
  op.b = atoi(s);
  while (!isspace(*s)) {
    s++;
  }
  s++;
  assert(isdigit(*s));
  op.c = atoi(s);
  return op;
}


static void exec(const enum OpCode op, const int a, const int b, const int c,
                 int registers[static 4]) {
  switch (op) {
  case OP_ADD_R:
    registers[c] = registers[a] + registers[b];
    break;
  case OP_ADD_I:
    registers[c] = registers[a] + b;
    break;
  case OP_MUL_R:
    registers[c] = registers[a] * registers[b];
    break;
  case OP_MUL_I:
    registers[c] = registers[a] * b;
    break;
  case OP_BAN_R:
    registers[c] = registers[a] & registers[b];
    break;
  case OP_BAN_I:
    registers[c] = registers[a] & b;
    break;
  case OP_BOR_R:
    registers[c] = registers[a] | registers[b];
    break;
  case OP_BOR_I:
    registers[c] = registers[a] | b;
    break;
  case OP_SET_R:
    registers[c] = registers[a];
    break;
  case OP_SET_I:
    registers[c] = a;
    break;
  case OP_GT_IR:
    registers[c] = a > registers[b] ? 1 : 0;
    break;
  case OP_GT_RI:
    registers[c] = registers[a] > b ? 1 : 0;
    break;
  case OP_GT_RR:
    registers[c] = registers[a] > registers[b] ? 1 : 0;
    break;
  case OP_EQ_IR:
    registers[c] = a == registers[b] ? 1 : 0;
    break;
  case OP_EQ_RI:
    registers[c] = registers[a] == b ? 1 : 0;
    break;
  case OP_EQ_RR:
    registers[c] = registers[a] == registers[b] ? 1 : 0;
    break;

  }
}

int main(void) {
  clock_t t = clock_time();

  // parse instructions
  char buf[1024];
  fgets(buf, 1024, stdin);
  const unsigned int ipr = (unsigned) (buf[4] - '0');
  struct Op instructions[64];
  int n = 0;
  while (fgets(buf, 1024, stdin) && n < 64) {
    instructions[n++] = parse_op(buf);
  }

  // run part 1
  int registers[6] = {0, 0, 0, 0, 0, 0};
  while (registers[ipr] < n) {
    struct Op op = instructions[registers[ipr]];
    exec(op.code, op.a, op.b, op.c, registers);
    registers[ipr]++;
  }
  int pt1 = registers[0];

  // run part 2
  memcpy(registers, (int[]){1, 0, 0, 0, 0, 0}, 6 * sizeof(int));
  while (registers[ipr] < n) {
    if (registers[ipr] == 1) {
      break;
    }
    struct Op op = instructions[registers[ipr]];
    exec(op.code, op.a, op.b, op.c, registers);
    registers[ipr]++;
  }

  // register #2 contains the number for which we want the sum of its divisors
  const unsigned int r = (unsigned int) registers[2];
  const unsigned int ur = r / 2;

  // every number is divisible by 1 and itself, so we can start there
  unsigned int pt2 = 1 + r;
  for (unsigned int i = 2; i < ur; i++) {
    if (r % i == 0) {
      pt2 += i;
    }
  }

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", pt1);
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
