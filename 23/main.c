#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

#define NDEBUG 1 

const char *test_input = "389125467";
const char *input = "327465189";

typedef struct cup cup_t;
struct cup {
    int64_t label;
    cup_t *next;
};

void
parse_input(cup_t *cups, const char *s) {
    size_t i = 0;
    cup_t *c;

    while (*s != '\0') {
        c = &cups[i++];
        c->label = (*s++ - '0');
        c->next = &cups[i];
    }

    // fill in remainder with incrementing values
    while (i < 1000000) {
        c = &cups[i++];
        c->label = i;
        c->next = i < 1000000 ? &cups[i] : &cups[0];
    }
}

void 
print_cups(cup_t *head, cup_t *highlight_cup, size_t n, char *before) {
    printf("%s", before);
    cup_t *c = head;
    for (size_t i=0; i < n; i++, c = c->next) {
        if (c == highlight_cup) {
            printf("%s(%ld)", i > 0 ? ", " : "", c->label);
        } else {
            printf("%s%ld", i > 0 ? ", " : "", c->label);
        }
    }
    printf("\n");
}

int main() {
    cup_t *cups = malloc(1000000 * sizeof(cup_t));
    parse_input(cups, input);

    cup_t *head = &cups[0];
    cup_t *current_cup = head;
    
    cup_t *selection = NULL;
    cup_t *destination = NULL;

    cup_t **lookup_table = malloc(1000001 * sizeof(cup_t *));
    for (int i=0; i < 1000000; i++) {
        lookup_table[cups[i].label] = &cups[i];
    }

    // 100 moves
    for (size_t m=0; m < 10000000; m++) {
        #ifndef NDEBUG
        printf("-- move %ld --\n", m+1);
        print_cups(head, current_cup, 12, "cups: ");
        #endif

        selection = current_cup->next;
        current_cup->next = selection->next->next->next;
        #ifndef NDEBUG
        print_cups(selection, NULL, 3, "pick up: ");
        #endif

        int64_t label = current_cup->label;
        while (1) {
            label = label > 1 ? label - 1 : 1000000;

            if (label != selection->label && label != selection->next->label && label != selection->next->next->label) {
                destination = lookup_table[label];
                break;
            }
        }
        #ifndef NDEBUG
        printf("destination: %ld\n\n", destination->label);
        #endif

        // insert cups after destination
        selection->next->next->next = destination->next;
        destination->next = selection;

        // select next cup as current cup
        current_cup = current_cup->next;
    }

    cup_t *c = lookup_table[1]->next;
    printf("%ld * %ld = %ld\n", c->label, c->next->label, c->label * c->next->label);
    
    free(lookup_table);
    free(cups);
}
