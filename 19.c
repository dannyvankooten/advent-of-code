#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rule {
  char ch;
  bool is_char;
  int32_t main[3];
  int32_t nmain;
  int32_t alt[3];
  int32_t nalt;
  int32_t index;
};
typedef struct rule rule_t;

int32_t message_matches_rule(rule_t rule,
                             rule_t* rules,
                             char* m,
                             int32_t depth) {
  // for (int i=0; i < depth; i++) printf("    |");
  // print_rule(rule);
  // printf("   (msg = %s)", m);
  // printf("\n");

  if (rule.is_char) {
    return *m == rule.ch ? 1 : 0;
  }

  // first, try main branch
  int32_t characters_matched = 0;
  int32_t inc;
  int32_t r;
  char* tmp = m;
  int32_t rid;

  for (r = 0; r < rule.nmain; r++) {
    rid = rule.main[r];
    rule_t main = rules[rid];
    inc = message_matches_rule(main, rules, m, depth + 1);
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
    for (r = 0; r < rule.nalt; r++) {
      rid = rule.alt[r];
      rule_t alt = rules[rid];
      inc = message_matches_rule(alt, rules, m, depth + 1);
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
  FILE* f = fopen("inputs/19.txt", "r");
  if (!f) {
    err(EXIT_FAILURE, "error reading input file");
  }
  char linebuf[BUFSIZ] = {0};

  // parse rules
  rule_t* rules = malloc(200 * sizeof(rule_t));
  if (!rules) {
    err(EXIT_FAILURE, "error allocating memory for rules");
  }
  for (int32_t i = 0; i < 200; i++) {
    rules[i].nmain = 0;
    rules[i].nalt = 0;
    rules[i].index = 0;
    for (int8_t j = 0; j < 3; j++) {
      rules[i].main[j] = 0;
      rules[i].alt[j] = 0;
    }
  }
  int32_t nrules = 0;

  while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
    char* s = linebuf;

    // parse rule index
    int32_t rule_idx = 0;
    while (isdigit(*s)) {
      rule_idx = (rule_idx * 10) + (*s - '0');
      s++;
    }
    rule_t* r = &rules[rule_idx];
    r->index = rule_idx;
    if (rule_idx >= nrules) {
      nrules = rule_idx + 1;
    }

    s += 2;  // ": "

    // parse 1st set options
    int32_t i;
    r->is_char = false;
    for (i = 0; *s != '\n' && *s != '|' && *s != '\0';) {
      if (*s == '"' && isalpha(*(s + 1))) {
        r->is_char = true;
        r->ch = *(s + 1);
        s += 2;
        break;
      } else {
        r->main[i] = 0;
        while (isdigit(*s)) {
          r->main[i] = (r->main[i] * 10) + (*s - '0');
          s++;
        }
        i++;
        if (*s == ' ')
          s++;
      }
    }
    r->nmain = i;
    while (*s == ' ')
      s++;
    if (*s == '|') {
      s += 2;  // "| "
      for (i = 0; *s != '\n' && *s != '\0';) {
        r->alt[i] = 0;
        while (isdigit(*s)) {
          r->alt[i] = (r->alt[i] * 10) + (*s - '0');
          s++;
        }
        i++;
        if (*s == ' ')
          s++;  // ' '
      }
      r->nalt = i;
    }
  }

  // parse messages
  rule_t zero = rules[0];
  // assert(message_matches_rule(zero, rules, "bbabbbbaabaabba", 0) > 0);
  // assert(message_matches_rule(zero, rules, "ababaaaaaabaaab", 0) > 0);
  // assert(message_matches_rule(zero, rules, "ababaaaaabbbaba", 0) > 0);
  // assert(message_matches_rule(zero, rules, "babbbbaabbbbbabbbbbbaabaaabaaa",
  // 0) > 0 && 4); assert(message_matches_rule(zero, rules,
  // "bbbbbbbaaaabbbbaaabbabaaa", 0) > 0); assert(message_matches_rule(zero,
  // rules, "bbbababbbbaaaaaaaabbababaaababaabab", 0) > 0 && 5);
  // assert(message_matches_rule(zero, rules, "baabbaaaabbaaaababbaababb", 0) >
  // 0); assert(message_matches_rule(zero, rules,
  // "abbbbabbbbaaaababbbbbbaaaababb", 0) > 0);
  // assert(message_matches_rule(zero, rules, "aaaaabbaabaaaaababaa", 0) > 0);
  // assert(message_matches_rule(zero, rules,
  // "aaaabbaabbaaaaaaabbbabbbaaabbaabaaa", 0) > 0);
  // assert(message_matches_rule(zero, rules,
  // "aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba", 0) > 0);

  int32_t count = 0;
  while (fgets(linebuf, BUFSIZ, f) != NULL) {
    // check if message matches rule 0
    if (message_matches_rule(zero, rules, linebuf, 0) > 0) {
      count++;
    }
  }
  printf("%d\n", count);
  assert(count == 350);

  fclose(f);
  free(rules);
  return 0;
}