#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int32_t count_yes_answers(char* str) {
  char group_answers[26];
  char user_answers[26] = {0};
  for (int8_t i = 0; i < 26; i++)
    group_answers[i] = -1;

  while (*str >= 'a' && *str <= 'z') {
    user_answers[*str - 'a'] = 1;
    str++;

    if (*str == '\n' || *str == '\0') {
      // go over 'a' to 'z' and mark them as answered if in user_answers
      for (int8_t i = 0; i < 26; i++) {
        group_answers[i] = user_answers[i] == 1 && (group_answers[i] == 1 ||
                                                    group_answers[i] == -1)
                               ? 1
                               : 0;
      };

      // if we reached end of string, break
      if (*str == 0) {
        break;
      }

      for (int8_t i = 0; i < 26; i++)
        user_answers[i] = 0;
      str++;
    }
  }

  int32_t y_count = 0;
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

  // Actual input
  FILE* f = fopen("inputs/06.input", "r");
  if (!f) {
    err(EXIT_FAILURE, "Error reading input file.");
  }

  char line_buf[BUFSIZ] = {0};
  char group_buf[BUFSIZ] = {0};
  int32_t sum = 0;

  while (fgets(line_buf, BUFSIZ, f) != NULL) {
    if (*line_buf == '\n') {
      sum += count_yes_answers(group_buf);
      group_buf[0] = '\0';
      continue;
    }

    strcat(group_buf, line_buf);
  }

  // last group
  sum += count_yes_answers(group_buf);

  fclose(f);
  printf("%d\n", sum);
  return 0;
}
