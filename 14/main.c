#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>

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

    for (int i=0; i < 36; i++) {
        // get bit in n at position i
        k = (n >> (35-i)) & 1;

        // if mask[i] equals 'X', take bit from number
        if ((mask[i] == 'X' && k == 1) || mask[i] == '1') {
            r += (unsigned long) pow(2.00, (double) 35-i);
        } 
    }

    return r;
}

unsigned long long bin2dec(char mask[36]) {
    unsigned long long r = 0;

    for (int i=0; i < 36; i++) {
        if (mask[i] == '1') {
            r += (unsigned long long) pow(2.00, (double) 35-i);
        } 
    }
    return r;
}

struct vec {
    unsigned long *values;
    int size;
    int cap;
};

void 
apply_address_mask(struct vec *v, char mask[36], unsigned long n) {
    // generate all possible combinations
    char k;
    #define MASK_CAP 1024
    char masks[MASK_CAP][36];
    int nmasks = 1;
    
    // TODO: Lazy copy here (vs. overwriting all nmasks at every step)
    for (int i=0; i < 36; i++) {        
        switch (mask[i]) {
            case '0':
                k = (n >> (35-i)) & 1;
                for (int m=0; m < MASK_CAP; m++) {
                    masks[m][i] = k == 1 ? '1' : '0';
                }
                break;
            case '1':
                for (int m=0; m < MASK_CAP; m++) {
                    masks[m][i] = '1';
                }
                break;
            case 'X': {
                for (int m=0; m < MASK_CAP; m++) {
                    masks[m][i] = '1';
                }

                // create new variation of each nmask, with i set to 0
                int nstop = nmasks;
                for (int n=0; n < nstop; n++) {
                    for (int j=0; j < i; j++) {
                        masks[nmasks][j] = masks[n][j];
                    }
                    masks[nmasks][i] = '0';
                    nmasks++;

                    if (nmasks >= MASK_CAP) {
                        err(EXIT_FAILURE, "Trying to add more than %d masks", MASK_CAP);
                    }
                }
              
                }
                break;
        }
    }

    // allocate enough memory
    if (nmasks > v->cap) {
        v->cap = nmasks;
        v->values = realloc(v->values, v->cap * sizeof(unsigned long));
    }

    v->size = 0;
    for (int i=0; i < nmasks; i++) {
        v->values[v->size++] = bin2dec(masks[i]);
    }
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
   // assert(apply_bitmask("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X", 11) == 73);

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
            while (*s != 0 && *(s - 2) != '=') s++;
            while (*s != '\0' && *s != '\n') {
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
            while (*s != '=' && *s != 0) s++;
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
    struct node {
        unsigned long address;
        unsigned int value;
    };
    size_t mem_size = 0;
    size_t mem_cap = 1024;
    struct node *memory = malloc(mem_cap * sizeof (struct node));

    // walk through instructions
    struct vec addresses = {
        .size = 0,
        .cap = 256,
        .values = malloc(256 *sizeof(unsigned long)),
    };
    char mask[36];
    for (int i = 0; i < size; i++) {
        #ifdef STEP
        printf("\n%d  \tmask=\t\t%.36s\n", i, mask);
        #endif
        
        switch (instructions[i].type) {
            case SET_MASK:
                strncpy(mask, instructions[i].mask, 36);
            break;

            case SET_VALUE: {
                //unsigned long result = apply_bitmask(mask, instructions[i].value);
                #ifdef STEP
                printf("address= \t\t%ld \t", instructions[i].address);
                print_binary(instructions[i].address);
                printf("\n");
                #endif

                apply_address_mask(&addresses, mask, instructions[i].address);
                for (int a=0; a < addresses.size; a++) {
                    size_t m = 0;
                    for (; m < mem_size; m++) {
                        if (memory[m].address == addresses.values[a]) {
                            memory[m].value = instructions[i].value;
                            break;
                        }
                    }
                    if (m == mem_size) {
                        // add to memory
                        if (mem_size >= mem_cap) {
                            mem_cap *= 2;
                            memory = realloc(memory, mem_cap * sizeof(struct node));
                        }
                        memory[mem_size++] = (struct node) {
                            .address = addresses.values[a],
                            .value = instructions[i].value,
                        };
                    }
                    // printf("Setting memory address %ld to %ld\n", addresses.values[a], instructions[i].value);
                    // memory[addresses.values[a]] = instructions[i].value;
                }
            }
            break;
        }        
    }

    unsigned long long sum = 0;
    for (size_t m = 0; m < mem_size; m++) {
        sum += memory[m].value;
    }
    printf("Sum: %lld\n", sum);
}
