#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>

enum instruction_types {
    NOOP,
    ACC,
    JUMP
};

struct Instruction {
    enum instruction_types type;
    char sign;
    int value;
};

struct Instructions {
    struct Instruction *values;
    size_t cap;
    size_t size;
};

struct Instruction 
parse_instruction_line(char *line) 
{
    char buf[32];
    int i = 0;
    while (*line != ' ') {
        buf[i++] = *line++;
    };
    buf[i] = '\0';

    struct Instruction ins;
    if (strcmp(buf, "nop") == 0) {
        ins.type = NOOP;
    } else if(strcmp(buf, "acc") == 0) {
        ins.type = ACC;
    } else if (strcmp(buf, "jmp") == 0) {
        ins.type = JUMP;
    } else {
        err(EXIT_FAILURE, "Invalid op type");
    }

    // skip spaces
    while (*line == ' ') line++;

    // read sign
    ins.sign = *line == '+' ? 1 : -1;
    line++;

    // skip spaces
    while (*line == ' ') line++;

    // read value
    int v = 0;
    while (*line != '\n' && *line != '\0') {
        v = v * 10 + (*line - '0');
        line++;
    }
    ins.value = v * ins.sign;
    return ins;    
}

int day8() {
    const char *file = "08.input"; 
    struct Instructions ins = {
        .cap = 64,
        .size = 0,
        .values = malloc(64 * sizeof (struct Instruction)),
    };
    if (!ins.values) {
        err(EXIT_FAILURE, "Error allocating memory for instruction values");
    }

    // Parse input file
    FILE *f = fopen(file, "r");
    if (!f) {
        err(EXIT_FAILURE, "error reading input file");
    }
    char linebuf[BUFSIZ] = {0};
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        struct Instruction i = parse_instruction_line(linebuf);
        if (ins.size == ins.cap) {
            ins.cap *= 2;
            ins.values = realloc(ins.values, ins.cap * sizeof(struct Instruction));
            if (!ins.values) {
                err(EXIT_FAILURE, "Error allocating memory for instruction values");
            }
        }
        ins.values[ins.size++] = i;
    }
    fclose(f);

    int *seen = malloc(ins.size * sizeof(int));
    if (!seen) {
        err(EXIT_FAILURE, "error allocating memory for seen array");
    }
    struct Instruction i;

    for (size_t c=0; c < ins.size; c++) {
        int acc = 0;
        size_t ip;
        memset(seen, 0, ins.size * sizeof(0));

        for (ip=0; ip < ins.size; ) {
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
                continue; // continue without increment instruction pointer
                break;
            }

            ip++;
        }

        // Print result if we made it to end of program
        if (ip == ins.size) {
            printf("%d\n", acc);
        }
        
    }

    free(ins.values);
    free(seen);
    return 0;
}
