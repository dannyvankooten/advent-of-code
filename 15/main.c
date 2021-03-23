#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>

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
    int diff;
    int i, j;

    int *seen = malloc(sizeof(int) * N);
    memset(seen, -1, sizeof(int) * N);
        
    int nnumbers = 0;
    for (i=1; numbers[i] != -1; i++, nnumbers++) {        
        seen[numbers[i-1]] = i-1;
    } 

    for (i=nnumbers; i < N; i++) {
        j = seen[numbers[i-1]];
        diff = j >= 0 ? i - 1 - j : 0;
        numbers[nnumbers++] = diff;
        seen[numbers[i-1]] = i-1;
    }

    printf("%d'th number = %d", N, numbers[N-1]);
}
