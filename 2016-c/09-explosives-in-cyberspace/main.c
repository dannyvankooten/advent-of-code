#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 9: Explosives in Cyberspace"

size_t pt1(const char *input) {
    char *s = input;
    int len;
    int repeats;
    char *begin;
    char *buf = malloc_or_die(128 * 1024);

    while (*s != 0x0) {

        if (*s == '(') {
            begin = s;
            s = skip("(", s);
            s = parse_int(&len, s);
            s = skip("x", s);
            s = parse_int(&repeats, s);
            s = skip(")", s);

            // the next len chars should be repeated repeat times

            // first, create some space
            // memmove(s + (size_t)(len * repeats), s, strlen(s) + 1);

            strcpy(buf, s);
            s = begin;
            for (int i = 0; i < repeats; i++) {
                memcpy(s, buf, (size_t)len);
                s += len;
            }

            memcpy(s, buf + (size_t)len, strlen(buf));
            s -= 1;
        }

        s++;
    }

    free(buf);
    return strlen(input);
}

size_t decompress(const char *input, size_t size) {
    size_t count = 0;
    int len;
    int repeats;

    const char *end = input + size;
    const char *s = input;
    while (s != end) {

        if (*s == '(') {
            s = skip("(", s);
            s = parse_int(&len, s);
            s = skip("x", s);
            s = parse_int(&repeats, s);
            s = skip(")", s);

            // then expanded by (repeats-1 * len)
            count = count + decompress(s, (size_t)len) * (size_t)repeats;
            s += len;
        } else {
            count++;
            s++;
        }
    }

    return count;
}

size_t pt2(char *input) { return decompress(input, strlen(input)); }

int main(void) {
    clock_t t = clock_time();

    char *input = malloc_or_die(128 * 1024);
    read_input_file(input, 128 * 1024, "input.txt");
    size_t a1 = pt1(input);
    size_t a2 = pt2(input);
    free(input);

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %ld\n", a1);
    printf("Part 2: %ld\n", a2);
    printf("Time: %.2fms\n", clock_time_since(t));
    return EXIT_SUCCESS;
}

// pt2: 2190468471 = too low
