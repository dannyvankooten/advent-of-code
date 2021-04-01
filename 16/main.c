#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>

struct rule {
    char type[64];
    int ranges[4];
    size_t position;
};

struct ticket {
    int values[32];
    size_t nvalues;
};

int
parse_digit(int *d, char *s) {
    char *start = s;
    while (*s == ' ') s++;

    char buf[8];
    char *b = buf;
    while (*s >= '0' && *s <= '9') {
        *b++ = *s++;
    }
    *b = '\0';
    *d = strtol(buf, NULL, 10);
    return s - start;
}

int day16() {
    struct rule *rules = malloc(sizeof(struct rule) * 100);
    size_t nrules = 0;
    // read input file
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    char buf[64];
    char *a, *b;

    // TODO: Parse rules into a single rule struct
    // parse rules
    while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
        a = linebuf;
        struct rule r;

        // parse type
        b = buf;
        while (*a != ':') {
            *b++ = *a++;
        }
        *b = '\0';
        strcpy(r.type, buf);
        a++;

        a += parse_digit(&r.ranges[0], a);
        while (*(a-1) != '-') a++;
        a += parse_digit(&r.ranges[1], a);        

        a = strstr(a, "or") + 2;
        a += parse_digit(&r.ranges[2], a);
        while (*(a-1) != '-') a++;
        a += parse_digit(&r.ranges[3], a);
        rules[nrules++] = r;
    }

    // parse "my ticket:"
    int d;
    while (fgets(linebuf, BUFSIZ, f) != NULL && strstr(linebuf, "your ticket:") != linebuf);
    struct ticket my_ticket;
    my_ticket.nvalues = 0;
    
    fgets(linebuf, BUFSIZ, f);
    a = linebuf;
    while (*a != '\n' && *a != '\0') {
        a += parse_digit(&d, a);
        my_ticket.values[my_ticket.nvalues++] = d;

        if (*a == ',') {
            a++; 
        } 
    }
    // for (int v=0; v < my_ticket.nvalues; v++) {
    //     printf("my_ticket.values[%d] = %d\n", v, my_ticket.values[v]);
    // }

    // skip foward to line saying "nearby tickets:"
    while (fgets(linebuf, BUFSIZ, f) != NULL && strstr(linebuf, "nearby tickets:") != linebuf);

    // // print rules
    // for (size_t i=0; i < nrules; i++) {
    //     printf("%s: %d - %d or %d - %d\n", rules[i].type, rules[i].ranges[0], rules[i].ranges[1], rules[i].ranges[2], rules[i].ranges[3]);
    // }
    char valid_any;

    struct ticket *nearby_tickets = malloc(260 * sizeof (struct ticket));
    for (size_t i=0; i < 260; i++) {
         nearby_tickets[i].nvalues = 0;
    };
    size_t ntickets = 0;

    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        a = linebuf;
        size_t nvalues = 0;

        // parse all digits on line
        while (*a != '\n' && *a != '\0') {
            a += parse_digit(&d, a);
            nearby_tickets[ntickets].values[nvalues++] = d;

            // loop over rules
            // if digit is not in any range, add to sum of invalid values
            valid_any = 0;
            for (size_t i=0; i < nrules; i++) {
                if (
                    (d >= rules[i].ranges[0] && d <= rules[i].ranges[1]) 
                    || (d >= rules[i].ranges[2] && d <= rules[i].ranges[3]) 
                    ) {
                    valid_any = 1;
                    break;
                }
            }
            if (0 == valid_any) {
                goto SKIPTICKET;
            }

            if (*a == ',') {
                a++; 
            }        
        }   
        
        // add ticket to list
        nearby_tickets[ntickets].nvalues = nvalues;
        ntickets++;

        // label for goto jump that skips tickets containing invalid values
        SKIPTICKET: ;
    }
    fclose(f);


    // find option for rule positions on ticket
    struct rule r;
    struct ticket t;
    int tval;

    size_t nvalues = nearby_tickets[0].nvalues;
    int *options = (int *) calloc(nrules * nvalues, sizeof(int));
    if (!options) err(EXIT_FAILURE, "could not allocate memory for options array");

    for (size_t i=0; i < nrules; i++) {
        r = rules[i];
        
        for (size_t j=0; j < nearby_tickets[0].nvalues; j++) {
            for (size_t k=0; k < ntickets; k++) {
                t = nearby_tickets[k];
                tval = t.values[j];
                if ( (tval < r.ranges[0] || tval > r.ranges[1]) && (tval < r.ranges[2] || tval > r.ranges[3]) ) {
                    goto NEXTPOS;
                }
            }

            options[i * nvalues + j] = 1;
            NEXTPOS: ;
        }
    }

    for (size_t i=0; i < nrules; i++) {
       r = rules[i];
       int position;
       int count = 0;

        for (size_t v=0; v < nvalues; v++) {            
            if (options[i * nvalues + v] == 1) {
                count++;
                position = v;

                // stop looking at this rule if we have 2 option availabilities
                if (count > 1) {
                    break;
                }
            }
        }
        
        // if we have exactly one option, use it
        // and discard it from all other rules
        if (count == 1) {
            char loopback = 0;
            for (size_t k=0; k < nrules; k++) {
                if (k == i) {
                    rules[i].position = position;
                    continue;
                }

                size_t idx = k * nvalues + position;
                if (options[idx] == 1) {
                    options[idx] = 0;
                    loopback = 1;
                }
            }
            if (loopback) {
                i = -1;
            }
        }
    }

    // finally, calculate product
    size_t product = 1;
    for (size_t i=0; i < nrules; i++) {
        r = rules[i];
        if (strstr(r.type, "departure") != r.type) {
            continue;
        }

        product *= my_ticket.values[r.position];
    }

    printf("%ld\n", product);

    free(options);
    free(rules);
    free(nearby_tickets);
    return 0;
}
