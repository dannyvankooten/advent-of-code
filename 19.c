#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

struct rule {
    char ch;
    char is_char;

    int main[3];
    int nmain;

    int alt[3];
    int nalt;
    
    int index;
};

typedef struct rule rule_t;

int 
message_matches_rule(rule_t rule, rule_t *rules, char *m, short depth) {
    // for (int i=0; i < depth; i++) printf("    |");
    // print_rule(rule);
    // printf("   (msg = %s)", m);
    // printf("\n");

    if (rule.is_char) {
        return *m == rule.ch ? 1 : 0;
    }

    // first, try main branch
    short characters_matched = 0;
    short inc;
    short r;
    char *tmp = m ;
    int rid;

    for (r=0; r < rule.nmain; r++) {
        rid = rule.main[r];
        rule_t main = rules[rid];
        inc = message_matches_rule(main, rules, m, depth+1); 
        if (inc == 0) {
            break;
        } else {
            m += inc;
            characters_matched += inc;

            // detect recursion for rules 31 and 42...
            if (r > 0 && (main.index == 31 || main.index == 42) && *m == '\n') {
                r = rule.nmain;
                break;
            }
        }
    }

    // if main failed, check against alt branch
    if (r < rule.nmain) {
        m = tmp;
        characters_matched = 0;
        for (r=0; r < rule.nalt; r++) {
            rid = rule.alt[r];
            rule_t alt = rules[rid];
            inc = message_matches_rule(alt, rules, m, depth+1); 
            if (inc == 0) {

                // detect recursion for rules 31 and 42...
                if (r > 0 && (alt.index == 31 || alt.index == 42)) {
                    r = rule.nalt;
                    break;
                }
                return 0;
            } else {
                m += inc;
                characters_matched += inc;
            }
        }

        if (r < rule.nalt) {
            return 0;
        }
    }

    if (rule.index == 0 && *m != '\n' && *m != '\0') {
        return 0;
    }

    // for (int i=0; i < depth; i++) printf("    |");
    // printf("OK! %d characters checked.\n", characters_matched);
    return characters_matched;
}


int day19() {
    FILE *f = fopen("19.input", "r");
    if (!f) {
        err(EXIT_FAILURE, "error reading input file");
    }
    char linebuf[BUFSIZ] = {0};

    // parse rules
    rule_t *rules = malloc(200 * sizeof(rule_t));
    for (int i=0; i < 200; i++) {
        for (int j=0; j<3; j++) {
            rules[i].main[j] = -1;
            rules[i].alt[j] = -1;
            rules[i].nmain = 0;
            rules[i].nalt = 0;
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
            rules[rule_idx].nmain = i;
        }

        if (*s == '|') {
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
                rules[rule_idx].nalt = i;
            }   

        }
    }

    // parse messages
    rule_t zero = rules[0];
    // assert(message_matches_rule(zero, rules, "bbabbbbaabaabba", 0) > 0);
    // assert(message_matches_rule(zero, rules, "ababaaaaaabaaab", 0) > 0);
    // assert(message_matches_rule(zero, rules, "ababaaaaabbbaba", 0) > 0);
    // assert(message_matches_rule(zero, rules, "babbbbaabbbbbabbbbbbaabaaabaaa", 0) > 0 && 4);
    // assert(message_matches_rule(zero, rules, "bbbbbbbaaaabbbbaaabbabaaa", 0) > 0);
    // assert(message_matches_rule(zero, rules, "bbbababbbbaaaaaaaabbababaaababaabab", 0) > 0 && 5);
    // assert(message_matches_rule(zero, rules, "baabbaaaabbaaaababbaababb", 0) > 0);
    // assert(message_matches_rule(zero, rules, "abbbbabbbbaaaababbbbbbaaaababb", 0) > 0);
    // assert(message_matches_rule(zero, rules, "aaaaabbaabaaaaababaa", 0) > 0);
    // assert(message_matches_rule(zero, rules, "aaaabbaabbaaaaaaabbbabbbaaabbaabaaa", 0) > 0);
    // assert(message_matches_rule(zero, rules, "aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba", 0) > 0);

    int count = 0;
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        // remove trailing linebreak
        //linebuf[strlen(linebuf)] = '\0';

        // check if message matches rule 0
        if (message_matches_rule(zero, rules, linebuf, 0) > 0) {
            count++;
        }
    }
    printf("%d\n", count);

    fclose(f);

    free(rules);
    return 0;
}

/*
"a" ("a" "a" )
*/