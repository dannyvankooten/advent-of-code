#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputs/06.h"

int8_t count_yes_answers(char* s) {
  char group_answers[26];
  char user_answers[26] = {0};
  for (int8_t i = 0; i < 26; i++) {
    group_answers[i] = -1;
  }

  while (*s >= 'a' && *s <= 'z') {
    user_answers[*s - 'a'] = 1;
    s++;

    if (*s == '\n' || *s == '\0') {
      // go over 'a' to 'z' and mark them as answered if in user_answers
      for (int8_t i = 0; i < 26; i++) {
        group_answers[i] = user_answers[i] == 1 && (group_answers[i] == 1 ||
                                                    group_answers[i] == -1)
                               ? 1
                               : 0;
      };

      // if we reached end of string, break
      if (*s == '\0') {
        break;
      }

      for (int8_t i = 0; i < 26; i++) {
        user_answers[i] = 0;
      }
      s++;
    }
  }

  int8_t y_count = 0;
  for (int8_t i = 0; i < 26; i++) {
    if (group_answers[i] == 1) {
      y_count++;
    }
  }

  return y_count;
}

int day6() {
  // Test input
  assert(count_yes_answers("abc") == 3);
  assert(count_yes_answers("a\nb\nc") == 0);
  assert(count_yes_answers("ab\nac") == 1);
  assert(count_yes_answers("a\na\na\na\n") == 1);
  assert(count_yes_answers("b") == 1);

  const unsigned char *s = input;
  char group_buf[BUFSIZ] = {0};
  int32_t sum = 0;
  char *s2 = group_buf;

  while (*s != '\0') {
    // copy line into groupbuf
    while (*s != '\n' && *s != '\0') {
      *s2++ = *s++;
    }
    *s2 = '\0';

    if (*s == '\n') s++; //newline

     // if empty newline
    if (*s == '\n') {
      sum += count_yes_answers(group_buf);
      // reset groupbuf
      s2 = group_buf;
      s++;
    }
  }

  // last group
  printf("%d\n", sum);
  // assert(sum == 6387);
  return 0;
}
