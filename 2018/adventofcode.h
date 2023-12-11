#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

void read_input_file(char *dest, char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        fprintf(stderr, "error reading %s", file);
        exit(EXIT_FAILURE);
    }
    size_t nread = fread(dest, 1, 64*1024, fp);

    while (dest[nread-1] == '\n' || dest[nread-1] == 0x0) nread--;
    dest[nread] = '\0';
    fclose(fp);
}

// parse_ident parses a string consisting of only lowercase a-z chars
char *parse_ident(char *dst, char *s) {
    while (*s >= 'a' && *s <= 'z') {
        *dst++ = *s++;
    }
    *dst = '\0';
    return s;
}

// parse_int parses a (signed) integer, with optional plus sign
char *parse_int(int *dst, char *s) {
    int n = 0;
    int mod = 1;
    if (*s == '-') {
        mod = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }
    *dst = n * mod;
    return s;
}


// parse_uint8 parses a single byte
char *parse_uint8(uint8_t *dst, char *s) {
    uint8_t n = 0;

    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }
    *dst = n;
    return s;
}

// skip advanced the pointer s as long as it matches the corresponding chars from expects
char *skip(char *expects, char *s) {
    while (*s == *expects) {
        s++;
        expects++;
    };

    if (*expects != '\0') {
        printf("Parse error. Expected '%c', got '%c'.\n%s\n", *expects, *s, s);
        exit(EXIT_FAILURE);
    }
    return s;
}

char *skip_optional(char c, char *s) {
    while (*s == c) {
        s++;
    }

    return s;
}

char *skip_until(char c, char *s) {
    while (*s != c && *s != 0x0) {
        s++;
    }

    return s;
}

clock_t timer_start() {
    return clock();
}

double timer_stop(clock_t start_t) {
    clock_t end_t = clock();
    return (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
}