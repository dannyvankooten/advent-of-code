#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};

    // parse "ready at timestamp" from 1st line
    fgets(linebuf, BUFSIZ, f) != NULL ?: err(EXIT_FAILURE, "invalid input");
    int ready_timestamp = strtol(linebuf, NULL, 10);
    printf("Ready at timestamp %d\n", ready_timestamp);

    // parse bus schedules from 2nd line
    fgets(linebuf, BUFSIZ, f) != NULL ?: err(EXIT_FAILURE, "invalid input");
    char *s = linebuf;
    char nbuf[8] = {0};
    int i = 0;
    int buses[64] = {[0 ... 63] = 0};
    int nbuses = 0;
    while (*s != '\n' && *s != '\0') {
        while(*s != ',') {
            nbuf[i++] = *s++;
        }
        nbuf[i] = '\0';
        if (*nbuf != 'x') {
            buses[nbuses++] = strtol(nbuf, NULL, 10);
        }
        i = 0;
        s++;
    }
    fclose(f);

    // find earliest bus we can take
    int bus_id = 0;
    int t = ready_timestamp;

    do {
        for (int b=0; b < nbuses; b++) {
            bus_id = buses[b];
            if (t % bus_id == 0) {
                goto BREAK;
            }
        }
    } while(t++);
    
    BREAK: ;
    printf("Bus %d at timestamp %d.\n", bus_id, t);
    printf("Result: %d\n", bus_id * (t - ready_timestamp));

}
