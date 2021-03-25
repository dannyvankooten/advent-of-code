#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

struct rule {
    char ch;
    int main[3];
    int alt[3];
    char is_char;
    char has_alt;
    int length;
    int index;
};

typedef struct rule rule_t;

void
print_rule(rule_t rule) {
    printf("Rule %d: ", rule.index);

    if (rule.is_char) {
        printf("\"%c\"", rule.ch);
        return;
    }

    for (int r=0; r < rule.length; r++) {
        printf("%d ", rule.main[r]);
    }

    if (rule.has_alt) {
        printf("| ");
   
        for (int r=0; r < rule.length; r++) {
            printf("%d ", rule.alt[r]);
        }
     }
}

int 
message_matches_rule(rule_t rule, rule_t *rules, char *m, short depth) {
    short characters_matched = 0;
    short inc;

    // protect against stack overflow
    if ( depth > 32000) {
        err(EXIT_FAILURE, "Depth > %d", 32000);
    }

    // for (int i=0; i < depth; i++) printf("\t");
    // print_rule(rule);
    // printf("\t(msg = %s)", m);
    // printf("\n");

    if (rule.is_char) {
        return *m == rule.ch ? 1 : 0;
    }

    // first, try main branch
    short r;
    char *tmp = m ;

    for (r=0; r < rule.length; r++) {
          rule_t main = rules[rule.main[r]];
          inc = message_matches_rule(main, rules, m, depth+1); 
          if (inc == 0) {
              break;
          } else {
              m += inc;
              characters_matched += inc;
          }
    }

    if (r < rule.length) {
        // reset *m
        // reset characters matched
        m = tmp;
        characters_matched = 0;
        for (r=0; r < rule.length; r++) {
            rule_t alt = rules[rule.alt[r]];
            inc = message_matches_rule(alt, rules, m, depth+1); 
            if (inc == 0) {
                return 0;
            } else {
                m += inc;
                characters_matched += inc;
            }
        }

        if (r != rule.length) {
            return 0;
        }
    }

    if (rule.index == 0 && *m != '\n' && *m != '\0') {
        return 0;
    }

    // for (int i=0; i < depth; i++) printf("\t");
    // printf("OK! %d characters checked.\n", characters_matched);
    return characters_matched;
}


int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        err(EXIT_FAILURE, "error reading input file");
    }
    char linebuf[BUFSIZ] = {0};

    // parse rules
    rule_t *rules = malloc(200 * sizeof(rule_t));
    for (int i=0; i<200; i++) {
        for (int j=0; j<3; j++) {
            rules[i].main[j] = -1;
            rules[i].alt[j] = -1;
            rules[i].length = 0;
            rules[i].has_alt = 0;
            rules[i].index = 0;
        }
    }
    int nrules = 0;
    
    while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
        char *s = linebuf;
        char tmp[64];

        // parse rule index
        char *t = tmp;
        while (*s != ':') {
            *t++ = *s++;
        }
        *t = '\0';
        int rule_idx = (int) strtol(tmp, NULL, 10);
        rules[rule_idx].index = rule_idx;
        if (rule_idx >= nrules) {
            nrules = rule_idx + 1;
        }

        s++; // :

        // parse options
        for (int i=0; *s != '\n' && *s != '|' && *s != '\0';) {
            while (*s == ' ' || *s == '"') s++;
            if (isalpha(*s)) {
                rules[rule_idx].is_char = 1;
                rules[rule_idx].ch = *s++;
            } else {
                rules[rule_idx].is_char = 0;
                t = tmp;
                while (isdigit(*s)) {
                    *t++ = *s++;
                }
                *t = '\0';
                rules[rule_idx].main[i++] = strtol(tmp, NULL, 10);
            }
            while (*s == ' ' || *s == '"') s++;
            rules[rule_idx].length = i;
        }

        if (*s == '|') {
            rules[rule_idx].has_alt = 1;
            s++;
            for (int i=0; *s != '\n' && *s != '\0';) {
                while (*s == ' ' || *s == '"') s++;
                t = tmp;
                while (isdigit(*s)) {
                    *t++ = *s++;
                }
                *t = '\0';
                rules[rule_idx].alt[i++] = strtol(tmp, NULL, 10);
                while (*s == ' ' || *s == '"') s++;
            }
        }
    }


    // print rules
    for (int i=0; i < nrules; i++) {
        print_rule(rules[i]);
        printf("\n");
    }

    // parse messages
    rule_t zero = rules[0];
    // assert(message_matches_rule(zero, rules, "ababbb", 0) > 0);
    // assert(message_matches_rule(zero, rules, "abbbab", 0) > 0);
    // assert(message_matches_rule(zero, rules, "bababa", 0) == 0);
    // assert(message_matches_rule(zero, rules, "aaabbb", 0) == 0);
    // assert(message_matches_rule(zero, rules, "aaaabbb", 0) == 0);


    int count = 0;
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        // remove trailing linebreak
        //linebuf[strlen(linebuf)] = '\0';

        // check if message matches rule 0
        if (message_matches_rule(zero, rules, linebuf, 0) > 0) {
            count++;
        }
    }
    printf("%d messages matched rule 0.\n", count);

    fclose(f);

    free(rules);
}

/*
"a" ("a" "a" )
*/