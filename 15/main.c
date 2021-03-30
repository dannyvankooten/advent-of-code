#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include "hashmap.h"

#define N 30000000

size_t   
parse_input(int_fast32_t *numbers, char *s) {
    int_fast32_t num;
    size_t n = 0;

    while (*s != '\0') {
        num = 0;
        while (*s >= '0' && *s <= '9') {
            num = (num * 10) + (*s - '0');
            s++;
        }

        numbers[n++] = num;
        if (*s == ',') s++;
    }
    
    return n;
}

int main() {
    int_fast32_t *numbers = malloc(sizeof (int_fast32_t) * N);;
    size_t nnumbers = parse_input(numbers, "12,1,16,3,11,0");
    int_fast32_t *seen = malloc(sizeof(int_fast32_t) * N);
    if (!seen) err(EXIT_FAILURE, "could not allocate memory for lookup table");
    memset(seen, -1, sizeof(int_fast32_t) * N);
        
    size_t i;
    for (i=0; i < nnumbers; i++) {
        seen[numbers[i]] = i;
    }

    int_fast32_t last_number;
    int_fast32_t diff;
    int_fast32_t j;
    size_t last_i;

    for (; i < N; i++) {
        last_i = i - 1;
        last_number = numbers[last_i];
        j = seen[last_number];
        seen[last_number] = last_i;
        diff = j > -1 ? last_i - j : 0;
        numbers[i] = diff;
    }

    printf("%d'th number = %" PRId64, N, numbers[N-1]); 

    free(numbers);
    free(seen);  
}
