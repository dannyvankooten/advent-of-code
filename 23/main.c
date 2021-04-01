#include <stdint.h>
#include <stdio.h>
#include <err.h>
#include <assert.h>

#define NDEBUG 1 
// const char *test_input = "389125467";
const char *input = "327465189";

void
print_raw(int32_t *cups);

static 
int32_t
parse_input(int32_t *cups, const char *s) {
    int32_t i = 1;
    int32_t first = (*s++ - '0');
    int32_t prev = first;

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
print_raw(int32_t *cups) {
    printf("| ");
    for(int32_t i = 1; i <= 9; i++) {
        printf("%2d | ", i);
    }
    printf("\n| ");
    for(int32_t i = 1; i <= 9; i++) {
        printf("%2d | ", cups[i]);
    }
    printf("\n\n");
}

void 
print_cups(int32_t *cups, int32_t first, size_t n, char *before) {
    printf("%s", before);
    int32_t c = first;
    for (size_t i=0; i < n; i++, c = cups[c]) {
        printf("%s%d", i > 0 ? ", " : "", c);
    }
    printf("\n");
}

int day23() {
    // int32_t *cups =  malloc(1000001 * sizeof(int32_t));
    int32_t cups[1000001]; 
    int32_t current_cup = parse_input(cups, input);
    int32_t a, b, c;
    int32_t destination;
    assert(current_cup == 3);
    int32_t next_cur;

    for (int32_t m=0; m < 10000000; m++) {
        // printf("-- move %ld --\n", m+1);
        // print_raw(cups);
        // print_cups(cups, current_cup, 9, "cups: ");
        a = cups[current_cup];
        b = cups[a];
        c = cups[b];

        // printf("selection: %ld, %ld, %ld\n", a, b, c);
        cups[current_cup] = next_cur = cups[c];

        destination = current_cup;
        do {
            destination--;
            if (destination < 1) {
                destination = 1000000;
            }
        }  while (destination == a || destination == b || destination == c);
        
        // printf("destination %ld\n", destination);
        cups[c] = cups[destination];
        cups[destination] = a;
        current_cup = next_cur;
    }

    int32_t c1 = cups[1];
    int32_t c2 = cups[c1];
    uint64_t r = (uint64_t) c1 * (uint64_t) c2;
    printf("%ld\n", r);
    assert(r == 474600314018);
    // free(cups);
    return 0;
}
