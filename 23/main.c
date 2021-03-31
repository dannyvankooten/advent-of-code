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
print_raw(u_int64_t *cups);

u_int64_t
parse_input(u_int64_t *cups, const char *s) {
    size_t i = 1;
    u_int64_t first = (*s++ - '0');
    u_int64_t prev = first;

    while (*s != '\0') {
        cups[prev] = (*s++ - '0');
        prev = cups[prev];
        i++;
    }

    // fill in remainder with incrementing values
    while (i < 1000000) {
        cups[prev] = ++i;
        prev = cups[prev];
    }

    // last element should point  to first
    cups[prev] = first;

    return first;
}

void
print_raw(u_int64_t *cups) {
    printf("| ");
    for(size_t i = 1; i <= 9; i++) {
        printf("%2ld | ", i);
    }
    printf("\n| ");
    for(size_t i = 1; i <= 9; i++) {
        printf("%2ld | ", cups[i]);
    }
    printf("\n\n");
}

void 
print_cups(u_int64_t *cups, u_int64_t first, size_t n, char *before) {
    printf("%s", before);
    u_int64_t c = first;
    for (size_t i=0; i < n; i++, c = cups[c]) {
        printf("%s%ld", i > 0 ? ", " : "", c);
    }
    printf("\n");
}

int main() {
    u_int64_t *cups =  malloc(1000001 * sizeof(u_int64_t));
    u_int64_t current_cup = parse_input(cups, input);
    u_int64_t selection[3];

    assert(current_cup == 3);

    for (size_t m=0; m < 10000000; m++) {
        // printf("-- move %ld --\n", m+1);
        // print_raw(cups);
        // print_cups(cups, current_cup, 9, "cups: ");

        selection[0] = cups[current_cup];
        selection[1] = cups[selection[0]];
        selection[2] = cups[selection[1]];

        // printf("selection: %ld, %ld, %ld\n", selection[0], selection[1], selection[2]);
        cups[current_cup] = cups[selection[2]];

        u_int64_t destination = current_cup;
        do {
            destination--;
            if (destination < 1) {
                destination = 1000000;
            }
        }  while (destination == selection[0] || destination == selection[1] || destination == selection[2] );
        
        // printf("destination %ld\n", destination);
        cups[selection[2]] = cups[destination];
        cups[destination] = selection[0];
        current_cup = cups[current_cup];
    }

    u_int64_t c1 = cups[1];
    u_int64_t c2 = cups[c1];
    printf("%ld * %ld = %ld\n", c1, c2, c1 * c2);
    free(cups);
    return 0;
}
