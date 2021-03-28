#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include "hashmap.h"

#define N 30000000

int_fast32_t * 
parse_input(char *input) {
    int_fast32_t *numbers = malloc(sizeof (int_fast32_t) * N);
    size_t nnumbers = 0;
    char *s = input;
    int_fast16_t num;

    while (*s != '\0') {
        if (*s == ',') s++;

        num = 0;
        while (*s >= '0' && *s <= '9') {
            num = (num * 10) + (*s - '0');
            s++;
        }

        numbers[nnumbers++] = num;
    }
    
    return numbers;
}

int main() {
    int_fast32_t *numbers = parse_input("12,1,16,3,11,0");
    int_fast32_t *seen = malloc(sizeof(int_fast32_t) * N);
    if (!seen) err(EXIT_FAILURE, "could not allocate memory for lookup table");
    memset(seen, -1, sizeof(int_fast32_t) * N);
        
    int_fast32_t i;
    for (i=1;numbers[i-1] != 0; i++) {   
        seen[numbers[i-1]] = i-1;    
    } 
    int_fast32_t last_number;
    int_fast32_t diff;
    int_fast32_t j;
    int_fast32_t last_i;

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
