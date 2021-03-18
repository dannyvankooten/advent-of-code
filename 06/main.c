#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

int
count_yes_answers(char *str) {
    char group_answers[26];
    char user_answers[26];
    for (int i=0; i<26; user_answers[i] = 0, group_answers[i] = -1, i++);

    while (*str >= 'a' && *str <= 'z') {
        user_answers[*str - 'a'] = 1;
        str++;

        if (*str == '\n' || *str == '\0') {
            // go over 'a' to 'z' and mark them as answered if in user_answers
            for (int i=0; i < 26; i++) {
                group_answers[i] = user_answers[i] == 1 && (group_answers[i] == 1 || group_answers[i] == -1) ? 1 : 0;
            };
            
            // if we reached end of string, break
            if (*str == 0) {
                break;
            }

            for (int i=0; i<26; user_answers[i++] = 0);
            str++;
        }
    }

    int y_count = 0;
    for (int i=0; i < 26; i++) {
        if (group_answers[i] == 1) {
            y_count++;
        }
    }

    return y_count;
}

int main() {
    // Test input
    assert(count_yes_answers("abc") == 3);
    assert(count_yes_answers("a\nb\nc") == 0);
    assert(count_yes_answers("ab\nac") == 1);
    assert(count_yes_answers("a\na\na\na\n") == 1);
    assert(count_yes_answers("b") == 1);

    // Actual input
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "Error reading input file.");

    char line_buf[BUFSIZ] = {0};
    char group_buf[BUFSIZ] = {0};
    int sum = 0;

    while(fgets(line_buf, BUFSIZ, f) != NULL) {
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
    printf("Sum: %d\n", sum);
}
