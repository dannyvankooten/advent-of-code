#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

enum {
    OP_HALF,
    OP_TRIPLE,
    OP_INC,
    OP_JMP,
    OP_JUMP_IFE,
    OP_JUMP_IF1,
};
char *opcode_names[] = {
   "hlf",
   "tpl",
   "inc",
   "jmp",
   "jie",
   "jio",
};

char *parse_int(int *dst, char *s) {
    int n = 0;
    int mod = *s == '-' ? -1 : 1;
    s++;

    while (*s >= '0' && *s <= '9') {
        n = n * 10 + *s - '0';
        s++;
    }

    n *= mod;
    *dst = n;
    return s;
}

int solve(int codes[][3], int ncodes, int registers[2]) {
    int i = 0;

    while (1) {
        if (i >= ncodes || i < 0) {
            break;
        }

        int *ins = codes[i];
        switch (ins[0]) {
            case OP_HALF:
                registers[ins[1] - 'a'] /= 2;
                break;
            case OP_TRIPLE:
                registers[ins[1] - 'a'] *= 3;
                break;
            case OP_INC:
                registers[ins[1] - 'a'] += 1;
                break;
            case OP_JMP:
                i += ins[1];
                continue;
            case OP_JUMP_IF1:
                if (registers[ins[1] - 'a'] == 1) {
                    i += ins[2];
                    continue;
                }
                break;
            case OP_JUMP_IFE:
                if (registers[ins[1] - 'a'] % 2 == 0) {
                    i += ins[2];
                    continue;
                }
                break;
        }

        i++;
    }

    return registers['b' - 'a'];
}


int parse(char *filename, int codes[64][3]) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    char input[1024*32];
    size_t nread = fread(input, 1, 1024*32, fp);
    input[nread] = '\0';
    fclose(fp);

    int ncodes = 0;
    char *s = input;
    while (*s != 0) {
        if (strncmp(s, "hlf", 3) == 0) {
            codes[ncodes][0] = OP_HALF;
        } else if (strncmp(s, "tpl", 3) == 0) {
            codes[ncodes][0] = OP_TRIPLE;
        } else if (strncmp(s, "inc", 3) == 0) {
            codes[ncodes][0] = OP_INC;
        } else if (strncmp(s, "jmp", 3) == 0) {
            codes[ncodes][0] = OP_JMP;
        } else if (strncmp(s, "jie", 3) == 0) {
            codes[ncodes][0] = OP_JUMP_IFE;
        } else if (strncmp(s, "jio", 3) == 0) {
            codes[ncodes][0] = OP_JUMP_IF1;
        }

        // skip code + " "
        s += 4;

        if (*s == 'a' || *s == 'b') {
            codes[ncodes][1] = *s;
            s++;
        } else {
            s = parse_int(&codes[ncodes][1], s);
        }

        if (*s == ',') {
            s += 2;
            s = parse_int(&codes[ncodes][2], s);
        }

        ncodes++;
        if (*s == '\n') s++;
    }

    return ncodes;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    int codes[64][3];
    int ncodes = parse("input.txt", codes);
    int a1 = solve(codes, ncodes, (int [2]){0, 0});
    int a2 = solve(codes, ncodes, (int [2]){1, 0});

    printf("--- Day 23: Opening the Turing Lock ---\n");
    printf("Part 1: %d\n", a1);
    printf("Part 2: %d\n", a2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}