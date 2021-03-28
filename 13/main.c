#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <limits.h>

int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};

    // parse "ready at timestamp" from 1st line
    if(fgets(linebuf, BUFSIZ, f) == NULL) err(EXIT_FAILURE, "invalid input");
    int ready_timestamp = strtol(linebuf, NULL, 10);
    printf("Ready at timestamp %d\n", ready_timestamp);

    // parse bus schedules from 2nd line
    if (fgets(linebuf, BUFSIZ, f) == NULL) err(EXIT_FAILURE, "invalid input");
    char *s = linebuf;
    char nbuf[8] = {0};
    int buses[64] = {0};
    int nbuses = 0;
    while (*s != '\n' && *s != '\0') {
        int i = 0;
        while(*s == 'x' || (*s >= '0' && *s <= '9')) {
            nbuf[i++] = *s++;
        }
        nbuf[i] = '\0';
        i = 0;

        buses[nbuses++] = *nbuf == 'x' ? 1 : (int) strtol(nbuf, NULL, 10);
        if (*s == ',') s++; // skip comma
    }
    fclose(f);

    // // find idx of highest bus ID (so we can use that to increment search step)
    // int highest_bus_id_idx = 0;
    // for (int i=0; i < nbuses; i++) {
    //     if (buses[i] > buses[highest_bus_id_idx]) {
    //         highest_bus_id_idx = i;
    //     }
    // }

    // int t2;
    // for (unsigned long long t=buses[highest_bus_id_idx]-highest_bus_id_idx; t < ULLONG_MAX - buses[highest_bus_id_idx] - 1; t += buses[highest_bus_id_idx]) {
    //     for (i =0, t2=t; i < nbuses && (buses[i] == 1 || t2 % buses[i] == 0); i++, t2++);
        
    //     if (i >= nbuses - 1) {
    //         printf("t = %lld\n", t);
    //         break;
    //     } 
    // }       

    // Above solution takes several minutes... Had to look up Chinese remainder Theorem.
    // https://en.wikipedia.org/wiki/Chinese_remainder_theorem
    unsigned long long t = 0;
    unsigned long long step = (unsigned long long) buses[0];
    for (int b=0; b < nbuses; b++) {
        while (1) {
            if ((t + b) % buses[b] == 0) {
                step *= buses[b];
                break;
            }
            
            t += step;
        }
    }
    printf("Result: %lld\n", t);

}
