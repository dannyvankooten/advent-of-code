#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

void pt1(char *s) {
    uint8_t counts[26] = {0};
    int twos = 0;
    int threes = 0;

    while (*s != '\0') {
        if (*s == '\n') {
            // find twos and threes
            uint8_t c2 = 0;
            uint8_t c3 = 0;
            for (uint8_t i=0; i < 26; i++) {
                if (counts[i] == 2) {
                    c2 = 1;
                } else if (counts[i] == 3) {
                    c3 = 1;
                }
            }

            memset(counts, 0, 26 * sizeof(uint8_t));
            twos += c2;
            threes += c3;
        } else {
            counts[(*s)-'a']++;
        }

        s++;
    }

    printf("%d\n", twos * threes);
}

void pt2(char *s) {
    // 27 chars per line (incl. trailing \n)
    // 250 lines
    // 25 must match
    char common[26] = {0};
    uint8_t ncommon = 0;
    for (uint8_t i = 0; i < 250; i++) {
        for (uint8_t j = i; j < 250; j++) {
            for (uint8_t k = 0; k < 26; k++) {
                if (s[i * 27 + k] == s[j * 27 + k]) {
                    common[ncommon++] = s[j * 27 + k];
                }
            }

            if (ncommon == 25) {
                goto end;
            }

            ncommon = 0;
        }
    }

    end:
    common[ncommon] = '\0';
    printf("%s\n", common);
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        abort();
    }
    char input[32*1024];
    size_t nread = fread(input, 1, 32*1024, fp);
    input[nread] = '\0';
    fclose(fp);

    pt1(input);
    pt2(input);
}
