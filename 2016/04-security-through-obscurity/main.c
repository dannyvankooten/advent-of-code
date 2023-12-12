#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void read_input(char *dst) {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading input.txt");
        exit(EXIT_FAILURE);
    }
    size_t nread = fread(dst, 1, 64 * 1024, fp);
    fclose(fp);
    dst[nread] = '\0';
}

char *parse_int(int *dst, char *s) {
    int n = 0;
    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }

    *dst = n;
    return s;
}

void checksum_from_counts(char *dest, int counts[26]) {
    int max[5] = {0, 0, 0, 0, 0};

    for (int j = 0; j < 5; j++) {
        dest[j] = 'a';
        for (int i = 0; i < 26; i++) {
            if (counts[i] > max[j] || (counts[i] >= max[j] && (i + 'a') < dest[j])) {
                max[j] = counts[i];
                dest[j] = i + 'a';
            }
        }

        counts[dest[j] - 'a'] = 0;
    }

    dest[5] = '\0';
}

void decrypt(char *buf, int sectorid) {
    char *b = buf;
    while (*b != 0) {
        if (*b == '-') {
            *b++ = ' ';
        } else {
            *b = 'a' + (*b - 'a' + sectorid) % 26;
            b++;
        }
    }

    // trim trailing space
    if (b > buf && *(b-1) == ' ') {
        b--;
    }

    *b = '\0';
}

char is_checksum_valid(char *checksum, int counts[26]) {
    char buf[16];
    checksum_from_counts(buf, counts);
    return strncmp(checksum, buf, 5) == 0;
}

void solve(char *s, int *pt1, int *pt2) {
    int sum = 0;
    int counts[26];
    int sectorid = 0;
    char checksum[6];
    char buf[256];
    char *b;

    while (*s != '\0') {
        memset(counts, 0, 26 * sizeof(int));

        // parse encrypted room name and count chars
        b = buf;
        while (!(*s >= '0' && *s <= '9')) {
            if (*s >= 'a' && *s <= 'z') {
                counts[*s - 'a']++;
            }
            *b++ = *s++;
        }
        *b = 0;

        // parse sector id
        s = parse_int(&sectorid, s);

        // skip "["
        s++;

        // parse checksum
        char *c = checksum;
        while (*s != ']') {
            *c++ = *s++;
        }
        *c = 0;

        // skip "]"
        s++;

        if (is_checksum_valid(checksum, counts) == 1) {
            sum += sectorid;

            decrypt(buf, sectorid);
            if (strcmp("northpole object storage", buf) == 0) {
                *pt2 = sectorid;
            }
        }

        if (*s == '\n') s++;
    }

    *pt1 = sum;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[64 * 1024];
    read_input(input);

    int pt1, pt2;
    solve(input, &pt1, &pt2);
    printf("--- Day 4: Security Through Obscurity ---\n");
    printf("Part 1: %d\n", pt1);
    printf("Part 2: %d\n", pt2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
}
