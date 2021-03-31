#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <sys/types.h>

// #define NDEBUG 1 
const char *test_input = "389125467";
const char *input = "327465189";

void
print_raw(u_int32_t *cups);

u_int32_t
parse_input(u_int32_t *cups, const char *s) {
    size_t i = 1;
    u_int32_t first = (*s++ - '0');
    u_int32_t prev = first;

    for (;*s != '\0'; s++, i++) {
        cups[prev] = (*s - '0');
        prev = cups[prev];
    }

    // fill in remainder with incrementing values
    // for (i=i+1; i < 1000000; i++) {
    for (; i < 1000000; i++) {
        cups[prev] = i + 1;
        prev = cups[prev];
    }

    // last element should point  to first
    cups[prev] = first;

    return first;
}

void
print_raw(u_int32_t *cups) {
    printf("| ");
    for(u_int32_t i = 1; i <= 9; i++) {
        printf("%2d | ", i);
    }
    printf("\n| ");
    for(u_int32_t i = 1; i <= 9; i++) {
        printf("%2d | ", cups[i]);
    }
    printf("\n\n");
}

void 
print_cups(u_int32_t *cups, u_int32_t first, size_t n, char *before) {
    printf("%s", before);
    u_int32_t c = first;
    for (size_t i=0; i < n; i++, c = cups[c]) {
        printf("%s%d", i > 0 ? ", " : "", c);
    }
    printf("\n");
}

int main() {
    // u_int32_t *cups =  malloc(1000001 * sizeof(u_int32_t));
    u_int32_t cups[1000001]; // =  malloc(1000001 * sizeof(u_int32_t));
    u_int32_t current_cup = parse_input(cups, input);
    u_int32_t a, b, c;

    assert(current_cup == 3);

    for (size_t m=0; m < 10000000; m++) {
        // printf("-- move %ld --\n", m+1);
        // print_raw(cups);
        // print_cups(cups, current_cup, 9, "cups: ");

        a = cups[current_cup];
        b = cups[a];
        c = cups[b];

        // printf("selection: %ld, %ld, %ld\n", a, b, c);
        cups[current_cup] = cups[c];

        u_int32_t destination = current_cup;
        do {
            destination--;
            if (destination < 1) {
                destination = 1000000;
            }
        }  while (destination == a || destination == b || destination == c);
        
        // printf("destination %ld\n", destination);
        cups[c] = cups[destination];
        cups[destination] = a;
        current_cup = cups[current_cup];
    }

    u_int32_t c1 = cups[1];
    u_int32_t c2 = cups[c1];
    printf("%ld\n", (u_int64_t) c1 * c2);
    // free(cups);
    return 0;
}
