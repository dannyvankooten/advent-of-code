#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "hm_int.h"

struct instruction {
    enum {
        SET_MASK,
        SET_VALUE,
    } type;
    char mask[36];
    int64_t address;
    int64_t value;
};
typedef struct instruction instruction_t;

struct vec {
    int64_t *values;
    size_t size;
    size_t cap;
};
typedef struct vec vec_t;

void print_binary(unsigned long n);

unsigned long apply_bitmask(char mask[36], unsigned long n) {
    char k;
    unsigned long r = 0;

    for (int8_t i=0; i < 36; i++) {
        // get bit in n at position i
        k = (n >> (35-i)) & 1;

        // if mask[i] equals 'X', take bit from number
        if ((mask[i] == 'X' && k == 1) || mask[i] == '1') {
            r |= 1UL << (35-i);
        } 
    }

    return r;
}

unsigned long long bin2dec(char mask[36]) {
    unsigned long long r = 0;

    for (int8_t i=0; i < 36; i++) {
        if (mask[i] == '1') {
            r |= 1UL << (35-i);
        } 
    }
    return r;
}


void 
apply_address_mask(vec_t *v, char mask[36], unsigned long n) {
    // generate all possible combinations
    char k;
    size_t m;

    // reset vector
    v->size = 1;
    v->values[0] = 0;

    for (int8_t i=0; i < 36; i++) {        
        switch (mask[i]) {
            case '0':
                k = (n >> (35-i)) & 1;
                if (k == 1) {
                    for (m=0; m < v->size; m++) {
                        v->values[m] |= 1UL << (35-i);
                    }
                }
                break;
            case '1':
                for (m=0; m < v->size; m++) {
                    v->values[m] |= 1UL << (35-i);
                }
                break;
            case 'X': {
                size_t size_before = v->size; 
                // add each variation with i'th bit set to 0
                for (m=0; m < size_before; m++) {
                    v->values[v->size++] = v->values[m];
                }

                // and add each variation with it's bit set to 1
                for (m=0; m < size_before; m++) {
                    v->values[m] |= 1UL << (35-i);
                }
              
                }
                break;
        }
    }
}

void print_binary(unsigned long n) {
    int8_t c;
    int64_t k;
    for (c = 35; c >= 0; c--) {
        k = n >> c;

        if (k & 1) {
            printf("1");
        } else {
            printf("0");
        }
    }
}

int32_t day14() {    
   // assert(apply_bitmask("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X", 11) == 73);
    FILE *f = fopen("14.input", "r");
    if (!f) {
        err(EXIT_FAILURE, "error reading input file");
    }
    char linebuf[BUFSIZ] = {0};
    char nbuf[32] = {0};

    // parse program
    size_t cap = 592;
    size_t size = 0;
    instruction_t *instructions = malloc(cap * sizeof(instruction_t));
    if (!instructions) {
        err(EXIT_FAILURE, "error allocating memory for instructions");
    }
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        instruction_t *ins = &instructions[size++];
        char *s = linebuf;
        char *tmp = nbuf;

        if (strncmp(s, "mask = ", 7) == 0) {
            ins->type = SET_MASK;
            char *mask = ins->mask;
            s += 7; // skip "mask" 
            while (*s != 0 && *(s - 2) != '=') s++;
            while (*s != '\0' && *s != '\n') {
                *mask++ = *s++;
            }
        } else {
            ins->type = SET_VALUE;
            s += 3; // skip "mem"
            while (*(s-1) != '[') s++;
            while (*s != ']') *tmp++ = *s++;
            *tmp = '\0';
            ins->address = strtol(nbuf, NULL, 10);

            tmp = nbuf;
            while (*s != '=' && *s != 0) s++;
            while (*s < '0' || *s > '9') s++;
            while (*s >= '0' && *s <= '9') *tmp++ = *s++;
            *tmp = '\0';
            ins->value = strtol(nbuf, NULL, 10);
        }

        // add instruction to list
        if (size >= cap) {
            cap *= 2;
            instructions = realloc(instructions, cap * sizeof(instruction_t));
            if (!instructions) {
                err(EXIT_FAILURE, "error allocating memory for instructions");
            }
        }        
    }
    fclose(f);

    // initialize hashmap for storing memory
    hashmap_t *hm = hm_new(73000);
    // struct hashmap hm = hashmap_new();
    vec_t addresses = {
        .size = 0,
        .cap = 1024,
        .values = malloc(1024 * sizeof(int64_t)),
    };
    if (!addresses.values) {
        err(EXIT_FAILURE, "failed to allocate memory for addresses");
    }

    // walk through instructions
    char mask[36] = {0};
    size_t j;
    int64_t sum = 0;
    for (size_t i=0; i < size; i++) {
        #ifdef STEP
        printf("\n%d  \tmask=\t\t%.36s\n", i, mask);
        #endif
        
        switch (instructions[i].type) {
            case SET_MASK:
                for (j=0; j < 36; j++) {
                    mask[j] = instructions[i].mask[j];
                }
            break;

            case SET_VALUE: {
                #ifdef STEP
                printf("address= \t%ld \t", instructions[i].address);
                print_binary(instructions[i].address);
                printf("\n");
                #endif

                apply_address_mask(&addresses, mask, instructions[i].address);
                for (j=0; j < addresses.size; j++) {
                    sum += instructions[i].value;
                    sum -= hm_set(hm, addresses.values[j], instructions[i].value);
                    //sum = hashmap_set(&hm, );
                }
            }
            break;
        }        
    }

    // unsigned long sum = 0;
    // sum = hashmap_sum(&hm);
    printf("%ld\n", sum);
    assert(sum == 4173715962894);
   
    free(instructions);
    free(addresses.values);
    free(hm->entries);
    free(hm);
    return 0;
}
