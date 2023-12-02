#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int pt1(char *s) {
    int freq = 0;
    while (*s != '\0') {
        int mod = *s++ == '-' ? -1 : 1;

        int n = 0;
        while (*s >= '0' && *s <= '9') {
            n = (n * 10) + (*s++ - '0');
        }

        freq += (mod * n);

        // skip \n
        s++;
    }
    return freq;
}

int pt2(char *input) {
    int freq = 0;
    uint8_t seen[256*1024] = {0};
    start:
    char *s = input;
    int mod;
    int hash;
    int n;

    while (*s != '\0') {
        mod = *s++ == '-' ? -1 : 1;
        n = 0;
        while (*s >= '0' && *s <= '9') {
            n = (n * 10) + (*s++ - '0');
        }

        freq += (mod * n);
        hash = freq + 128 * 1024;
        if (seen[hash] == 1) {
            return freq;
        }
        seen[hash] = 1;

        // skip \n
        s++;
    }

    goto start;
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        abort();
    }
    char input[32*1024];
    size_t nread = fread(input, 1, 32*1024, fp);
    input[nread] = '\0';

    printf("%d\n", pt1(input));
    printf("%d\n", pt2(input));
}
