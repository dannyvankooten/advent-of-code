#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int str_is_nice(char *s) {
    int charcount[26] = {0};
    int repeats = 0;
    while (*s != '\n' && *s != '\0') {
        charcount[*s - 'a']++;

        if (*s == *(s+1)) {
            repeats++;
        }

        if (memcmp(s, "ab", 2) == 0 || memcmp(s, "cd", 2) == 0 || memcmp(s, "pq", 2) == 0 || memcmp(s, "xy", 2) == 0) {
            return 0;
        }

        s++;
    }

    int vowelcount = 0;
    char *vowels = "aeiou";
    int nvowels = 5;
    for (int i = 0; i < nvowels; i++) {
        vowelcount += charcount[vowels[i] - 'a'];

    }

    return repeats > 0 && vowelcount >= 3 ? 1 : 0;
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    char input[20*1024];
    size_t nread = fread(input, 1, 20*1024, fp);
    input[nread] = '\0';
    fclose(fp);

    char *s = input;
    int nice = 0;
    while (*s != '\0') {
        if (str_is_nice(s) == 1) {
            nice++;
        }

        while (*s != '\n' && *s != '\0') s++;
        if (*s == '\n') s++;
    }

    printf("%d\n", nice);
    return 0;
}
