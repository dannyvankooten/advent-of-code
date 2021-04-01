#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <limits.h>
#include <inttypes.h>

int day13() {
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};

    // parse "ready at timestamp" from 1st line
    if(fgets(linebuf, BUFSIZ, f) == NULL) err(EXIT_FAILURE, "invalid input");
    char *s = linebuf;
    int64_t ready_timestamp = 0;
    while (*s >= '0' && *s <= '9') {
        ready_timestamp = (ready_timestamp * 10) + (*s - '0');
        s++;
    }

    // parse bus schedules from 2nd line
    if (fgets(linebuf, BUFSIZ, f) == NULL) err(EXIT_FAILURE, "invalid input");
    s = linebuf;
    int64_t buses[64] = {0};
    size_t nbuses = 0;
    while (*s != '\n' && *s != '\0') {
        if (*s == 'x') {
            buses[nbuses++] = 1;
            s++;
        } else {
            int64_t n = 0;
            while (*s >= '0' && *s <= '9') {
                n = (n * 10) + (*s - '0');
                s++;
            }
            buses[nbuses++] = n;
        }

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
    int64_t t = 0;
    int64_t step = buses[0];
    for (size_t b=0; b < nbuses; b++) {
        while (1) {
            if ((t + b) % buses[b] == 0) {
                step *= buses[b];
                break;
            }
            
            t += step;
        }
    }
    printf("%" PRId64 "\n", t);
    assert(t == 415579909629976);
    return 0;
}
