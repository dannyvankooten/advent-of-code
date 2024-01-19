#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../adventofcode.h"

#define PUZZLE_NAME "Day 8: I Heard You Like Registers"

enum opcodes {
    OP_INC,
    OP_DEC,
    OP_GT,
    OP_GTE,
    OP_EQ,
    OP_NEQ,
    OP_LT,
    OP_LTE,
};

typedef struct {
    char (*reg_names)[4];
    int *reg_values;
    int nregs;
    int highest_value;
} cpu_t;

int *get(cpu_t *cpu, char *name) {
    for (int i = 0; i < cpu->nregs; i++) {
        if (strcmp(cpu->reg_names[i], name) == 0) {
            return &cpu->reg_values[i];
        }
    }

    strcpy(cpu->reg_names[cpu->nregs], name);
    cpu->reg_values[cpu->nregs] = 0;
    cpu->nregs++;
    return &cpu->reg_values[cpu->nregs-1];
}

char *parse_infix_op(char *dst, char *s) {
    if (strncmp(s, ">=", 2) == 0) {
        *dst = OP_GTE;
        s += 2;
    } else if (strncmp(s, "<=", 2) == 0) {
        *dst = OP_LTE;
        s += 2;
    } else if (strncmp(s, "==", 2) == 0) {
        *dst = OP_EQ;
        s += 2;
    } else if (strncmp(s, "!=", 2) == 0) {
        *dst = OP_NEQ;
        s += 2;
    } else if (strncmp(s, ">", 1) == 0) {
        *dst = OP_GT;
        s += 1;
    } else if (strncmp(s, "<", 1) == 0) {
        *dst = OP_LT;
        s += 1;
    }


    return s;
}

void exec(cpu_t *cpu, char *reg, int op, int opvalue, char *leftname, int infix, int right) {
    int *val = get(cpu, reg);
    int *left = get(cpu, leftname);

    int true = 0;
    switch (infix) {
        case OP_EQ:
            true = *left == right;
            break;
        case OP_NEQ:
            true = *left != right;
            break;
        case OP_GT:
            true = *left > right;
            break;
        case OP_GTE:
            true = *left >= right;
            break;
        case OP_LT:
            true = *left < right;
            break;
        case OP_LTE:
            true = *left <= right;
            break;
    }

    if (true) {
        switch (op) {
            case OP_INC:
                *val += opvalue;
                break;
            case OP_DEC:
                *val -= opvalue;
                break;
        }

        if (*val > cpu->highest_value) {
            cpu->highest_value = *val;
        }
    }
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[1024 * 64];
    read_input_file(input, "input.txt");

    // init cpu with plenty of registers
    cpu_t cpu;
    cpu.reg_names = malloc(26*26*26 * sizeof(*cpu.reg_names));
    if (cpu.reg_names == NULL) {
        exit(EXIT_FAILURE);
    }
    cpu.reg_values = calloc(26*26*26, sizeof(*cpu.reg_values));
    if (cpu.reg_values == NULL) {
        exit(EXIT_FAILURE);
    }
    cpu.nregs = 0;
    cpu.highest_value = -1;

    char target[4];
    char opcode;
    int opvalue;
    char left[4];
    int right;
    char infix_op;
    char *s = input;
    while (*s != 0x0) {
        // parse line
        s = parse_ident(target, s);
        s = skip(" ", s);
        opcode = (*s == 'i') ? OP_INC : OP_DEC;
        s += 3;
        s = skip(" ", s);
        s = parse_int(&opvalue, s);
        s = skip(" ", s);
        s = skip ("if ", s);
        s = parse_ident(left, s);
        s = skip(" ", s);
        s = parse_infix_op(&infix_op, s);
        s = skip(" ", s);
        s = parse_int(&right, s);
        while (*s != '\n' && *s != 0x0) s++;
        if (*s == '\n') s++;

        // execute instruction
        exec(&cpu, target, opcode, opvalue, left, infix_op, right);
    }

    int max = cpu.reg_values[0];
    for (int i = 1; i < cpu.nregs; i++) {
        if (cpu.reg_values[i] > max) {
            max = cpu.reg_values[i];
        }
    }

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", max);
    printf("Part 2: %d\n", cpu.highest_value);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);

    free(cpu.reg_names);
    free(cpu.reg_values);
    return EXIT_SUCCESS;
}
