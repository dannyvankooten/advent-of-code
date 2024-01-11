#include "adventofcode.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void checksum_from_counts(char dest[], int counts[26]) {
    int max[5] = {0, 0, 0, 0, 0};

    for (int j = 0; j < 5; j++) {
        dest[j] = 'a';
        for (int i = 0; i < 26; i++) {
            if (counts[i] > max[j] ||
                (counts[i] >= max[j] && (i + 'a') < dest[j])) {
                max[j] = counts[i];
                dest[j] = (char)i + 'a';
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
            *b = (char)('a' + (*b - 'a' + sectorid) % 26);
            b++;
        }
    }

    // trim trailing space
    if (b > buf && *(b - 1) == ' ') {
        b--;
    }

    *b = '\0';
}

char is_checksum_valid(char *checksum, int counts[26]) {
    char buf[16];
    checksum_from_counts(buf, counts);
    return strncmp(checksum, buf, 5) == 0;
}

void solve(const char *s, int *pt1, int *pt2) {
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

        if (*s == '\n')
            s++;
    }

    *pt1 = sum;
}

int main() {
    clock_t start_t = clock_time();

    char input[64 * 1024];
    read_input_file(input, 64 * 1024, "04.txt");

    int pt1, pt2;
    solve(input, &pt1, &pt2);
    printf("--- Day 4: Security Through Obscurity ---\n");
    printf("Part 1: %d\n", pt1);
    printf("Part 2: %d\n", pt2);
    printf("Time: %.2fms\n", clock_time_since(start_t));
}
