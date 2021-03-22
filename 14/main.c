#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include <math.h>

struct instruction {
    enum {
        SET_MASK,
        SET_VALUE,
    } type;
    char mask[36];
    unsigned long address;
    unsigned long value;
};

void print_binary(unsigned long n);

unsigned long apply_bitmask(char mask[36], unsigned long n) {
    char k;
    unsigned long r = 0;

    for (int i=35; i >= 0; i--) {
        // get bit in n at position i
        k = (n >> i) & 1;

        // if mask[i] equals 'X', take bit from number
        if ((mask[35-i] == 'X' && k == 1) || mask[35-i] == '1') {
            r += (unsigned long) pow(2.00, (double) i);
        } 
    }

    return r;
}

void print_binary(unsigned long n) {
    int c, k;
    for (c = 35; c >= 0; c--) {
        k = n >> c;

        if (k & 1) {
            printf("1");
        } else {
            printf("0");
        }
    }
}

int main() {
    assert(apply_bitmask("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X", 11UL) == 73);

    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    char nbuf[32] = {0};

    // parse program
    int cap = 64;
    int size = 0;
    struct instruction *instructions = malloc(64 * sizeof(struct instruction));
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        struct instruction ins;
        char *s = linebuf;
        char *tmp = nbuf;

        if (strncmp(s, "mask = ", 7) == 0) {
            ins.type = SET_MASK;
            char *mask = ins.mask;
            s += 7; // skip "mask" 
            while (*(s - 2) != '=') s++;
            while (*s != '\n' && *s != '\0') {
                *mask++ = *s++;
            }
        } else {
            ins.type = SET_VALUE;
            s += 3; // skip "mem"
            while (*(s-1) != '[') s++;
            while (*s != ']') *tmp++ = *s++;
            *tmp = '\0';
            ins.address = strtol(nbuf, NULL, 10);

            tmp = nbuf;
            while (*s != '=') s++;
            while (*s < '0' || *s > '9') s++;
            while (*s >= '0' && *s <= '9') *tmp++ = *s++;
            *tmp = '\0';
            ins.value = strtol(nbuf, NULL, 10);
        }

        // add instruction to list
        instructions[size++] = ins;
        if (size == cap) {
            cap *= 2;
            instructions = realloc(instructions, cap * sizeof(struct instruction));
        }
    }
    fclose(f);

    // initialize memory
    unsigned long memory[1000000] = { [0 ... 1000000-1] = 0};

    // walk through instructions
    char mask[36];
    for (int i = 0; i < size; i++) {
        #ifdef STEP
        printf("%d  \tmask=\t\t%.36s\n", i, mask);
        #endif
        
        switch (instructions[i].type) {
            case SET_MASK:
                strncpy(mask, instructions[i].mask, 36);
            break;

            case SET_VALUE:
                {
                unsigned long result = apply_bitmask(mask, instructions[i].value);
                #ifdef STEP
                printf("value= \t\t%ld \t", instructions[i].value);
                print_binary(instructions[i].value);
                printf("\n");
                printf("result= \t%ld \t", result);
                print_binary(result);
                printf("\n");
                #endif
                memory[instructions[i].address] = result;
                }
            break;
        }

        
    }

    unsigned long long sum = 0;
    for (int i=0; i < 1000000; sum += memory[i++]);
    printf("Sum: %lld\n", sum);
}
