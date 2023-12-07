#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define HASH(v) ((v[0] - 'a' + 1) * 27 + (v[1] == '\0' ? 0 : v[1] - 'a' + 1))

static inline
char *parse_int(uint16_t *dst, char *s) {
    uint16_t n = 0;
    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }
    *dst = n;
    return s;
}

static inline
char *parse_ident(char *dst, char *s) {
    while (*s >= 'a' && *s <= 'z') {
        *dst++ = *s++;
    }
    *dst = '\0';
    return s;
}

enum op {
    AND = 0,
    OR,
    NOT,
    LSHIFT,
    RSHIFT,
    DIRECT
};

char *opcode_to_str(enum op opcode) {
    char *names[] = {
        "AND",
        "OR",
        "NOT",
        "LSHIFT",
        "RSHIFT",
        "DIRECT",
    };

    return names[opcode];
}

static inline
char *parse_op(enum op *dst, char *s) {
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

    while (*s != ' ') s++;
    return s;
}

static inline
int find(int wires[], const char *target) {
    return wires[HASH(target)];
}

static inline
uint16_t *getsignal(int wires[], uint16_t *signals,  const char *target) {
    int pos = find(wires,  target);
    return pos > -1 ? &signals[pos] : NULL;
}

typedef struct instruction {
    char lhs_ident[3];
    char rhs_ident[3];
    char target[3];
    enum op opcode;
    uint16_t lhs_value;
    uint16_t rhs_value;
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
            printf("%s = %s %s %s\n", i.target, i.lhs_ident, opcode_to_str(i.opcode), i.rhs_ident);
        break;
    }
}

int run(instruction_t *instructions, int ninstructions) {
    int wires[27*27];
    memset(wires, -1, 27*27*4);
    uint16_t signals[340];
    uint16_t nwires = 0;

    while (1) {
        for (int i = 0; i < ninstructions; i++) {
            instruction_t *ins = &instructions[i];
            uint16_t *signal;

            // don't send to gates that already have a signal
            int pos = find(wires, ins->target);
            if (pos != -1) continue;

            // resolve signals
            if (strlen(ins->lhs_ident) > 0) {
                signal = getsignal(wires, signals, ins->lhs_ident);
                if (signal == NULL) continue;
                ins->lhs_value = *signal;
            }

            if (strlen(ins->rhs_ident) > 0) {
                signal = getsignal(wires, signals, ins->rhs_ident);
                if (signal == NULL) continue;
                ins->rhs_value = *signal;
            }

            uint16_t v = 0;
            switch (ins->opcode) {
                case DIRECT: {
                    v = ins->lhs_value;
                }
                break;

                case AND: {
                    v = ins->lhs_value & ins->rhs_value;
                }
                break;

                case OR: {
                    v = ins->lhs_value | ins->rhs_value;
                }
                break;

                case NOT: {
                    v = ~ ins->rhs_value;
                }
                break;

                case LSHIFT: {
                    v = ins->lhs_value << ins->rhs_value;
                }
                break;

                case RSHIFT: {
                    v = ins->lhs_value >> ins->rhs_value;
                }
                break;
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
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    char input[20*1024];
    size_t nread = fread(input, 1, 20*1024, fp);
    input[nread] = '\0';
    fclose(fp);

    clock_t start_t, end_t;
    start_t = clock();

    instruction_t instructions[340];
    int ninstructions = 0;

    // parse input
    char *s = input;
    while (*s != '\0') {
        instruction_t *ins = &instructions[ninstructions++];

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
         s += 3;

         // parse target wire
         s = parse_ident(ins->target, s);

         // skip to EOL
         s++;
    }

    printf("--- Day 7: Some Assembly Required ---\n");

    // run instructions
    uint16_t pt1 = run(instructions, ninstructions);
    printf("Part 1: %d\n", pt1);
    assert(pt1 == 46065);


    // override instructions so b takes direct value from signal a in part 1
    for (int i=0; i < ninstructions; i++) {
        if (strcmp(instructions[i].target, "b") == 0) {
            instructions[i].opcode = DIRECT;
            instructions[i].lhs_value = pt1;
            instructions[i].lhs_ident[0] = '\0';
            break;
        }
    }
    uint16_t pt2 = run(instructions, ninstructions);
    printf("Part 2: %d\n", pt2);
    assert(pt2 == 14134);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}