#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

struct rule {
    char type[64];
    int32_t ranges[4];
    size_t position;
};
typedef struct rule rule_t;

struct ticket {
    int32_t values[32];
    size_t nvalues;
};
typedef struct ticket ticket_t;

int32_t
parse_digit(int32_t *d, char *s) {
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

int32_t day16() {
    size_t nrules = 0;
    struct rule *rules = malloc(sizeof(rule_t) * 100);
    if (!rules) {
        err(EXIT_FAILURE, "error allocating memory for rules");
    }
    FILE *f = fopen("16.input", "r");
    if (!f) {
        err(EXIT_FAILURE, "error reading input file");
    }
    char linebuf[BUFSIZ] = {0};

    // parse rules
    while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
        char *a = linebuf;
        rule_t *r = &rules[nrules++];

        // parse type
        char *b = r->type;
        while (*a != ':') {
            *b++ = *a++;
        }
        *b = '\0';
        a++;

        // parse first range option
        a += parse_digit(&r->ranges[0], a);
        while (*(a-1) != '-') a++;
        a += parse_digit(&r->ranges[1], a);        

        // parse alternative range option
        a = strstr(a, "or") + 2;
        a += parse_digit(&r->ranges[2], a);
        while (*(a-1) != '-') a++;
        a += parse_digit(&r->ranges[3], a);
    }

    // parse "my ticket:"
    int32_t d;
    while (fgets(linebuf, BUFSIZ, f) != NULL && strstr(linebuf, "your ticket:") != linebuf);
    ticket_t my_ticket;
    my_ticket.nvalues = 0;
    
    fgets(linebuf, BUFSIZ, f);
    char *a = linebuf;
    while (*a != '\n' && *a != '\0') {
        a += parse_digit(&d, a);
        my_ticket.values[my_ticket.nvalues++] = d;

        if (*a == ',') {
            a++; 
        } 
    }
    // for (int32_t v=0; v < my_ticket.nvalues; v++) {
    //     printf("my_ticket.values[%d] = %d\n", v, my_ticket.values[v]);
    // }

    // skip foward to line saying "nearby tickets:"
    while (fgets(linebuf, BUFSIZ, f) != NULL && strstr(linebuf, "nearby tickets:") != linebuf);

    // // print32_t rules
    // for (size_t i=0; i < nrules; i++) {
    //     printf("%s: %d - %d or %d - %d\n", rules[i].type, rules[i].ranges[0], rules[i].ranges[1], rules[i].ranges[2], rules[i].ranges[3]);
    // }
    bool valid_any;
    ticket_t *nearby_tickets = malloc(260 * sizeof (ticket_t));
    if (!nearby_tickets) {
        err(EXIT_FAILURE, "error allocating memory for nearby tickets");
    }
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
            valid_any = false;
            for (size_t i=0; i < nrules; i++) {
                if (
                    (d >= rules[i].ranges[0] && d <= rules[i].ranges[1]) 
                    || (d >= rules[i].ranges[2] && d <= rules[i].ranges[3]) 
                    ) {
                    valid_any = true;
                    break;
                }
            }
            if (! valid_any) {
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
    ticket_t t;
    int32_t tval;

    size_t nvalues = nearby_tickets[0].nvalues;
    int32_t *options = (int32_t *) calloc(nrules * nvalues, sizeof(int));
    if (!options) {
        err(EXIT_FAILURE, "could not allocate memory for options array");
    }

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
       int32_t position;
       int32_t count = 0;

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
            bool loopback = 0;
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
    int64_t product = 1;
    for (size_t i=0; i < nrules; i++) {
        r = rules[i];
        if (strstr(r.type, "departure") != r.type) {
            continue;
        }

        product *= my_ticket.values[r.position];
    }

    printf("%ld\n", product);
    assert(product == 1346570764607);

    free(options);
    free(rules);
    free(nearby_tickets);
    return 0;
}
