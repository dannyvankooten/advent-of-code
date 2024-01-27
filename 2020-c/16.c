#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rule {
  char type[40];
  int32_t ranges[4];
  int32_t position;
};
typedef struct rule rule_t;

struct ticket {
  int16_t values[31];
  uint16_t nvalues;
};
typedef struct ticket ticket_t;

static int32_t 
parse_digit(int32_t* d, char* restrict s) {
  const char* start = s;
  while (*s == ' ') {
    s++;
  }

  *d = 0;
  while (*s >= '0' && *s <= '9') {
    *d = (*d * 10) + (*s - '0');
    s++;
  }
  return s - start;
}

int32_t day16() {
  int32_t nrules = 0;
  struct rule rules[128];
  FILE* f = fopen("inputs/16.txt", "r");
  if (!f) {
    err(EXIT_FAILURE, "error reading input file");
  }
  char linebuf[BUFSIZ] = {0};

  // parse rules
  while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
    char* a = linebuf;
    rule_t* r = &rules[nrules++];
    r->position = -1;

    // parse type
    char* b = r->type;
    while (*a != ':') {
      *b++ = *a++;
    }
    *b = '\0';
    a++;

    // parse first range option
    a += parse_digit(&r->ranges[0], a);
    while (*(a - 1) != '-')
      a++;
    a += parse_digit(&r->ranges[1], a);

    // parse alternative range option
    a = strstr(a, "or") + 2;
    a += parse_digit(&r->ranges[2], a);
    while (*(a - 1) != '-')
      a++;
    a += parse_digit(&r->ranges[3], a);
  }

  // parse "my ticket:"
  int32_t d;
  while (fgets(linebuf, BUFSIZ, f) != NULL &&
         memcmp(linebuf, "your ticket:", strlen("your ticket:")) != 0)
    ;
  ticket_t my_ticket;
  my_ticket.nvalues = 0;

  fgets(linebuf, BUFSIZ, f);
  char* a = linebuf;
  while (*a != '\n' && *a != '\0') {
    a += parse_digit(&d, a);
    my_ticket.values[my_ticket.nvalues++] = d;

    if (*a == ',') {
      a++;
    }
  }

  // skip foward to line saying "nearby tickets:"
  while (fgets(linebuf, BUFSIZ, f) != NULL && memcmp(linebuf, "nearby tickets:", strlen("nearby tickets:")) != 0);

  bool valid_any;
  ticket_t nearby_tickets[256];
  int32_t ntickets = 0;

  while (fgets(linebuf, BUFSIZ, f) != NULL) {
    a = linebuf;
    int32_t nvalues = 0;

    // parse all digits on line
    while (*a != '\n' && *a != '\0') {
      a += parse_digit(&d, a);
      nearby_tickets[ntickets].values[nvalues++] = d;

      // loop over rules
      // if digit is not in any range, add to sum of invalid values
      valid_any = false;
      for (int32_t i = 0; i < nrules; i++) {
        if ((d >= rules[i].ranges[0] && d <= rules[i].ranges[1]) ||
            (d >= rules[i].ranges[2] && d <= rules[i].ranges[3])) {
          valid_any = true;

          // no need to process any further if a single rule matches
          break;
        }
      }
      if (!valid_any) {
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
  SKIPTICKET:;
  }
  fclose(f);

  // find option for rule positions on ticket
  struct rule* r;
  int32_t tval;
  int32_t nvalues = nearby_tickets[0].nvalues;
  int options[nrules * nvalues];
  memset(options, 0, nrules * nvalues * sizeof(int));

  for (int32_t i = 0; i < nrules; i++) {
    r = &rules[i];

    for (int32_t j = 0; j < nearby_tickets[0].nvalues; j++) {
      for (int32_t k = 0; k < ntickets; k++) {
        tval = nearby_tickets[k].values[j];
        if ((tval < r->ranges[0] || tval > r->ranges[1]) &&
            (tval < r->ranges[2] || tval > r->ranges[3])) {
          goto NEXTPOS;
        }
      }

      options[i * nvalues + j] = 1;
    NEXTPOS:;
    }
  }

  REDUCE_RULES: ;
  bool loopback = false;
  for (int32_t i = 0; i < nrules; i++) {
    // skip rule if we already know position of its field
    if (rules[i].position >= 0) {
      continue;
    }

    int32_t position;
    int32_t count = 0;

    for (int32_t v = 0; v < nvalues; v++) {
      if (options[i * nvalues + v] == 1) {
        count++;
        position = v;

        // skip this rule if we have more than one option
        if (count > 1) {
          goto NEXTRULE;
        }
      }
    }

    // rule has exactly one option: use it
    // then discard this option from all other rules
    rules[i].position = position;
    loopback = true;

    // if we have exactly one option, use it
    // and discard it from all other rules
    for (int32_t k = 0; k < nrules; k++) {
      options[k * nvalues + position] = 0;
    }
    
    NEXTRULE: ;
  }
  if (loopback) {
    goto REDUCE_RULES;
  }
      

  // finally, calculate product
  int64_t product = 1;
  for (int32_t i = 0; i < nrules; i++) {
    if (memcmp(rules[i].type, "departure", 9) != 0) {
      continue;
    }

    product *= my_ticket.values[rules[i].position];
  }

  printf("%ld\n", product);
  assert(product == 1346570764607);
  return 0;
}
