#include "../adventofcode.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define HASH(v) ((v[0] - 'a' + 1) * 27 + (v[1] == '\0' ? 0 : v[1] - 'a' + 1))

enum op { AND = 0, OR, NOT, LSHIFT, RSHIFT, DIRECT };

static const char *opcode_names[] = {
    "AND", "OR", "NOT", "LSHIFT", "RSHIFT", "DIRECT",
};

static const char *opcode_to_str(enum op opcode) {
  return opcode_names[opcode];
}

static inline const char *parse_op(enum op *dst, const char *s) {
  switch (*s) {
  case 'A':
    *dst = AND;
    break;
  case 'O':
    *dst = OR;
    break;
  case 'N':
    *dst = NOT;
    break;
  case 'L':
    *dst = LSHIFT;
    break;
  case 'R':
    *dst = RSHIFT;
    break;
  }

  while (*s != ' ')
    s++;
  return s;
}

static inline int find(int wires[], const char *target) {
  return wires[HASH(target)];
}

static inline int *getsignal(int wires[], int *signals, const char *target) {
  int pos = find(wires, target);
  return pos > -1 ? &signals[pos] : NULL;
}

typedef struct instruction {
  char lhs_ident[3];
  char rhs_ident[3];
  char target[3];
  enum op opcode;
  int lhs_value;
  int rhs_value;
} instruction_t;

void print_instruction(instruction_t i) {
  switch (i.opcode) {
  case DIRECT:
    printf("%s = %s%d\n", i.target, i.lhs_ident, i.lhs_value);
    break;

  case NOT:
    printf("%s = %s %s\n", i.target, opcode_to_str(i.opcode), i.rhs_ident);
    break;

  case LSHIFT:
  case RSHIFT:
  case AND:
  case OR:
    printf("%s = %s %s %s\n", i.target, i.lhs_ident, opcode_to_str(i.opcode),
           i.rhs_ident);
    break;
  }
}

int run(instruction_t *instructions, int ninstructions) {
  int wires[27 * 27];
  memset(wires, -1, 27 * 27 * sizeof(int));
  int signals[512];
  int nwires = 0;

  while (1) {
    for (int i = 0; i < ninstructions; i++) {
      instruction_t *ins = &instructions[i];
      int *signal;

      // don't send to gates that already have a signal
      int pos = find(wires, ins->target);
      if (pos != -1)
        continue;

      // resolve signals
      if (*ins->lhs_ident != 0x0) {
        signal = getsignal(wires, signals, ins->lhs_ident);
        if (signal == NULL)
          continue;
        ins->lhs_value = *signal;
      }

      if (*ins->rhs_ident != 0x0) {
        signal = getsignal(wires, signals, ins->rhs_ident);
        if (signal == NULL)
          continue;
        ins->rhs_value = *signal;
      }

      int v = 0;
      switch (ins->opcode) {
      case DIRECT: {
        v = ins->lhs_value;
      } break;

      case AND: {
        v = ins->lhs_value & ins->rhs_value;
      } break;

      case OR: {
        v = ins->lhs_value | ins->rhs_value;
      } break;

      case NOT: {
        v = ~ins->rhs_value;
      } break;

      case LSHIFT: {
        v = ins->lhs_value << ins->rhs_value;
      } break;

      case RSHIFT: {
        v = ins->lhs_value >> ins->rhs_value;
      } break;
      }

      signals[nwires] = v;
      wires[HASH(ins->target)] = nwires;
      nwires++;

      if (nwires == ninstructions) {
        return signals[wires[HASH("a")]];
      }
    }
  }

  abort();
}

int main() {
  char input[32 * 1024] = "";
  read_input_file(input, 32 * 1024, "input.txt");
  clock_t start_t = clock_time();

  instruction_t instructions[512];
  int ninstructions = 0;

  // parse input
  const char *s = input;
  while (*s != '\0') {
    instruction_t *ins = &instructions[ninstructions++];
    ins->lhs_value = 0;
    ins->rhs_ident[0] = 0x0;
    ins->lhs_ident[0] = 0x0;

    if (*s >= '0' && *s <= '9') {
      s = parse_int(&ins->lhs_value, s);
    } else if (*s >= 'a' && *s <= 'z') {
      s = parse_ident(ins->lhs_ident, s);
    } else if (*s >= 'A' && *s <= 'Z') {
      s = parse_op(&ins->opcode, s);
    }

    // eat space
    s++;

    if (*s != '-') {
      // parse optional infix operator
      if (*s >= 'A' && *s <= 'Z') {
        s = parse_op(&ins->opcode, s);
        s++;
      }

      // parse rhs
      if (*s >= 'a' && *s <= 'z') {
        s = parse_ident(ins->rhs_ident, s);
      } else if (*s >= '0' && *s <= '9') {
        s = parse_int(&ins->rhs_value, s);
      }
      s++;
    } else {
      ins->opcode = DIRECT;
    }

    // skip "-> "
    s = skip("-> ", s);

    // parse target wire
    s = parse_ident(ins->target, s);

    // skip to EOL
    s++;
  }

  // run instructions
  int pt1 = run(instructions, ninstructions);

  // override instructions so b takes direct value from signal a in part 1
  for (int i = 0; i < ninstructions; i++) {
    if (strcmp(instructions[i].target, "b") == 0) {
      instructions[i].opcode = DIRECT;
      instructions[i].lhs_value = pt1;
      instructions[i].lhs_ident[0] = '\0';
      break;
    }
  }
  int pt2 = run(instructions, ninstructions);
  printf("--- Day 7: Some Assembly Required ---\n");
  printf("Part 1: %d %s\n", pt1, pt1 == 46065 ? "✔" : "");
  printf("Part 2: %d %s\n", pt2, pt2 == 14134 ? "✔" : "");
  printf("Time: %.2fms\n", clock_time_since(start_t));
  return 0;
}