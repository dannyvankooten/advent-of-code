#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

struct rule {
    char main[3];
    char alt[3];
    int length;
    char has_alt;
    int index;
};

typedef struct rule rule_t;

void
print_rule(rule_t rule) {
    printf("Rule %d: ", rule.index);
    for (int r=0; r < 3; r++) {
        if (rule.main[r] == -1) continue;
        printf("%c ", rule.main[r]);
    }

    if (rule.has_alt) {
        printf("| ");
    }
    for (int r=0; r < 3; r++) {
        if (rule.alt[r] == -1) continue;
        printf("%c ", rule.alt[r]);
    }
}

int 
message_matches_rule(rule_t rule, rule_t *rules, char *m, short depth) {
    short characters_matched = 0;
    short inc;

    if ( depth > 32000) {
        err(EXIT_FAILURE, "Depth > %d", 32000);
    }

    // for (int i=0; i < depth; i++) printf("\t");
    // print_rule(rule);
    // printf("\t(msg = %s)", m);
    // printf("\n");

    for (short r=0; r < rule.length; r++) {
        if (*m == '\0') {
            return 0;
        }

        if (isalpha(rule.main[r])) {
            if (*m != rule.main[r]) {
                return 0;
            } 

            characters_matched++;
            m++;
            continue;
        } else {
            // check against main branch
            rule_t main = rules[rule.main[r] - '0'];
            inc = message_matches_rule(main, rules, m, depth+1); 
            if (inc == 0) {
                if (rule.has_alt) {
                    rule_t alt = rules[rule.alt[r] - '0'];
                    inc = message_matches_rule(alt, rules, m, depth+1); 
                    if (inc == 0) {
                        return 0;
                    }
                } else {
                    return 0;
                }
            }

            m += inc;
            characters_matched += inc;
        }
    }

    if (rule.index == 0 && *m != '\n' && *m != '\0') {
        printf("Remainder: %s\n", m);
        return 0;
    }

    // for (int i=0; i < depth; i++) printf("\t");
    // printf("OK! %d characters checked.\n", characters_matched);

    return characters_matched;
}


int main() {
    FILE *f = fopen("test_input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
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
            rules[rule_idx].main[i++] = *s++;
            while (*s == ' ' || *s == '"') s++;
            rules[rule_idx].length = i;
        }

        if (*s == '|') {
            rules[rule_idx].has_alt = 1;
            s++;
            for (int i=0; *s != '\n' && *s != '\0';) {
                while (*s == ' ' || *s == '"') s++;
                rules[rule_idx].alt[i++] = *s++;
                while (*s == ' ' || *s == '"') s++;
            }
        }
    }


    // print rules
    for (int i=0; i < nrules; i++) {
        printf("%d: ", i);
        for (int r=0; r < rules[i].length; r++) {
            printf("%c ", rules[i].main[r]);
        }

        if (rules[i].has_alt) {
            printf("| ");

            for (int r=0; r < rules[i].length; r++) {
                printf("%c ", rules[i].alt[r]);
            }
        }
        
        printf("\n");
    }

    // parse messages
    rule_t zero = rules[0];
    assert(message_matches_rule(zero, rules, "ababbb", 0) > 0);
    assert(message_matches_rule(zero, rules, "abbbab", 0) > 0);
    assert(message_matches_rule(zero, rules, "bababa", 0) == 0);
    assert(message_matches_rule(zero, rules, "aaabbb", 0) == 0);
    assert(message_matches_rule(zero, rules, "aaaabbb", 0) == 0);


    int count = 0;
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        // remove trailing linebreak
        linebuf[strlen(linebuf) - 1] = '\0';

        // check if message matches rule 0
        if (message_matches_rule(zero, rules, linebuf, 0)) {
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