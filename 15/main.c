#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

char *test_input = "0,3,6";
char *input = "12,1,16,3,11,0";

int * 
parse_input(char *input) {
    int *numbers = malloc(sizeof (int) * 2020);
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
        
    int nnumbers = 0;
    for (i=0; numbers[i] != -1; i++, nnumbers++) {
        printf("%d, ", numbers[i]);
    } 

    #ifdef STEP
    system ("/bin/stty raw");
    #endif 

    for (i=nnumbers; i < 2020; i++) {
        // if this number appears for the first time, add 0 to list
        diff = 0;
        for (j=i-2; j >= 0; j--) {
            if (numbers[j] == numbers[i-1]) {
                diff = i - 1 - j;
                break;
            }
        }   
        numbers[nnumbers++] = diff;
        numbers[nnumbers] = -1;
        
        // otherwise, add difference between current position and last position
        printf("%d, ", numbers[i]);

        #ifdef STEP
        if (getchar() == 27) {
            break;
        }
        #endif
    }

    #ifdef STEP
    system ("/bin/stty cooked");
    #endif
}
