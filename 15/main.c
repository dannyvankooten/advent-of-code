#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include "hashmap.h"

#define N 30000000

int * 
parse_input(char *input) {
    int *numbers = malloc(sizeof (int) * N);
    int nnumbers = 0;

    char *s = input;
    char nbuf[32];
    char *n = nbuf;

    while (*s != '\0') {
        // skip comma
        if (*s == ',') s++;

        // read number
        while (*s != ',' && *s != '\0') {
            *n++ = *s++;
        }
        *n = '\0';

        // add to array
        numbers[nnumbers++] = strtol(nbuf, NULL, 10);
        n = nbuf;
    }
    
    numbers[nnumbers++] = -1; // add stopper element
    return numbers;
}

int main() {
    int *numbers = parse_input("12,1,16,3,11,0");
    int *seen = malloc(sizeof(int) * N);
    if (!seen) err(EXIT_FAILURE, "could not allocate memory for lookup table");
    memset(seen, -1, sizeof(int) * N);
        
    int nnumbers = 1;
    for (int i=1; numbers[i] != -1; i++, nnumbers++) {   
        seen[numbers[i-1]] = i-1;    
    } 
    int last_number;
    int diff;
    int j;

    for (size_t i=nnumbers, last_i=i-1; i < N; i++, last_i++) {
        last_number = numbers[last_i];
        j = seen[last_number];
        seen[last_number] = last_i;
        diff = j > -1 ? last_i - j : 0;
        numbers[nnumbers++] = diff;
    }

    printf("%d'th number = %d", N, numbers[N-1]);   
}
