#include "adventofcode.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 16: Chronal Classification"

enum OpCode {
  OP_UNKNOWN = -1,
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

static void exec(const enum OpCode op, const int a, const int b, const int c,
                 const int in[static 4], int out[static 4]) {
  switch (op) {
  case OP_ADD_R:
    out[c] = in[a] + in[b];
    break;
  case OP_ADD_I:
    out[c] = in[a] + b;
    break;
  case OP_MUL_R:
    out[c] = in[a] * in[b];
    break;
  case OP_MUL_I:
    out[c] = in[a] * b;
    break;
  case OP_BAN_R:
    out[c] = in[a] & in[b];
    break;
  case OP_BAN_I:
    out[c] = in[a] & b;
    break;
  case OP_BOR_R:
    out[c] = in[a] | in[b];
    break;
  case OP_BOR_I:
    out[c] = in[a] | b;
    break;
  case OP_SET_R:
    out[c] = in[a];
    break;
  case OP_SET_I:
    out[c] = a;
    break;
  case OP_GT_IR:
    out[c] = a > in[b] ? 1 : 0;
    break;
  case OP_GT_RI:
    out[c] = in[a] > b ? 1 : 0;
    break;
  case OP_GT_RR:
    out[c] = in[a] > in[b] ? 1 : 0;
    break;
  case OP_EQ_IR:
    out[c] = a == in[b] ? 1 : 0;
    break;
  case OP_EQ_RI:
    out[c] = in[a] == b ? 1 : 0;
    break;
  case OP_EQ_RR:
    out[c] = in[a] == in[b] ? 1 : 0;
    break;
  case OP_UNKNOWN:
    assert(0);
    break;
  }
}

static void parse_registers(const char line[static 1024], int dest[static 4]) {
  for (unsigned i = 0; i < 4; i++) {
    dest[i] = line[9 + i * 3] - '0';
  }
}

static struct Op parse_op(const char s[static 1024]) {
  struct Op op;

  op.code = atoi(s);
  while (!isspace(*s)) {
    s++;
  }

  op.a = atoi(++s);
  while (!isspace(*s)) {
    s++;
  }
  op.b = atoi(++s);
  while (!isspace(*s)) {
    s++;
  }
  op.c = atoi(++s);
  return op;
}

unsigned solve_pt1(void) {
  unsigned cnt = 0;

  char buf[1024];
  int registers_in[4];
  int registers_out[4];

  while (fgets(buf, 1024, stdin)) {
    if (buf[0] == '\n') {
      break;
    }

    // buf contains initial register state
    parse_registers(buf, registers_in);

    // next line contains Op
    assert(fgets(buf, 1024, stdin) != NULL);
    struct Op op = parse_op(buf);

    // next line contains final register state
    assert(fgets(buf, 1024, stdin) != NULL);
    parse_registers(buf, registers_out);

    // skip empty line
    assert(fgets(buf, 1024, stdin) != NULL);

    // perform all possible opcodes
    unsigned opcode_matches = 0;
    int out[4];
    memcpy(out, registers_in, 4 * sizeof(int));
    for (enum OpCode o = OP_ADD_R; o <= OP_EQ_RR; o++) {
      exec(o, op.a, op.b, op.c, registers_in, out);

      if (memcmp(registers_out, out, 4 * sizeof(int)) == 0) {
        opcode_matches++;
      }
    }

    if (opcode_matches >= 3) {
      cnt++;
    }
  }

  return cnt;
}


int solve_pt2(void) {
  char buf[1024];
  int registers_in[4];
  int registers_out[4];
  int opcode_to_usercode[16];
  int usercode_to_opcode[16];
  memset(opcode_to_usercode, OP_UNKNOWN, 16 * sizeof(*opcode_to_usercode));
  memset(usercode_to_opcode, OP_UNKNOWN, 16 * sizeof(*usercode_to_opcode));
  enum OpCode match;

  rewind(stdin);

  // first pass
  // determine all opcodes
  while (fgets(buf, 1024, stdin)) {
    if (buf[0] == '\n') {
      break;
    }

    // buf contains initial register state
    parse_registers(buf, registers_in);

    // next line contains Op
    assert(fgets(buf, 1024, stdin) != NULL);
    struct Op op = parse_op(buf);

    // next line contains final register state
    assert(fgets(buf, 1024, stdin) != NULL);
    parse_registers(buf, registers_out);

    // skip empty line
    assert(fgets(buf, 1024, stdin) != NULL);

    // skip if we already know what opcode this is
    if (usercode_to_opcode[op.code] != OP_UNKNOWN) {
      continue;
    }

    // perform all possible opcodes
    unsigned opcode_matches = 0;
    int out[4] = { registers_in[0], registers_in[1], registers_in[2], registers_in[3] };

    for (enum OpCode o = OP_ADD_R; o <= OP_EQ_RR; o++) {
      if (opcode_to_usercode[o] != OP_UNKNOWN) {
        continue;
      }

      exec(o, op.a, op.b, op.c, registers_in, out);

      if (memcmp(registers_out, out, 4 * sizeof(*registers_out)) == 0) {
        opcode_matches++;
        match = o;
      }
    }

    if (opcode_matches == 1) {
      opcode_to_usercode[match] = op.code;
      usercode_to_opcode[op.code] = match;

      // If we're still missing mappings at this point
      // We have to rewind stdin
      // But this was not necessary for my puzzle input
      //rewind(stdin);
    }
  }

 // skip empty line
  assert(fgets(buf, 1024, stdin) != NULL);

  memset(registers_in, 0, 4 * sizeof(*registers_in));
  while (fgets(buf, 1024, stdin) != NULL) {
    struct Op op = parse_op(buf);
    exec(usercode_to_opcode[op.code], op.a, op.b, op.c, registers_in, registers_in);
  }

  return registers_in[0];
}

int main(void) {
  clock_t t = clock_time();

  unsigned int pt1 = solve_pt1();
  int pt2 = solve_pt2();

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %d\n", pt1);
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
