#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

const char *test_input = "389125467";
const char *input = "327465189";

typedef struct cup cup_t;
struct cup {
    int8_t label;
    cup_t *next;
};

void
parse_input(cup_t *cups, const char *s) {
    size_t i = 0;

    while (*s != '\0') {
        cup_t *cup = &cups[i++];
        cup->label = (*s++ - '0');
        cup->next = &cups[i % 9];
    }
}

void 
print_cups(cup_t *head, cup_t *highlight_cup, size_t n, char *before) {
    printf("%s", before);
    cup_t *c = head;
    for (int i=0; i < n; i++, c = c->next) {
        if (c == highlight_cup) {
            printf("%s(%d)", i > 0 ? ", " : "", c->label);
        } else {
            printf("%s%d", i > 0 ? ", " : "", c->label);
        }
    }
    printf("\n");
}

cup_t *
find_cup_by_label(cup_t *head, int8_t label) {
    cup_t *c = head;
    for (size_t i = 0; i < 9; i++, c = c->next) {
        if (c->label == label) {
            return c;
        }
    }

    return NULL;
}

int main() {
    cup_t *cups = malloc(9 * sizeof(cup_t));
    parse_input(cups, input);

    cup_t *head = &cups[0];
    cup_t *current_cup = head;
    cup_t *selection = NULL;
    cup_t *destination = NULL;

    // 100 moves
    for (int8_t m=0; m < 100; m++) {
        printf("-- move %d --\n", m+1);
        print_cups(head, current_cup, 9, "cups: ");

        selection = current_cup->next;
        current_cup->next = selection->next->next->next;
        print_cups(selection, NULL, 3, "pick up: ");

        int8_t label = current_cup->label;
        destination = NULL;
        while (destination == NULL) {
            label--;
            label = label > 0 ? label : 9;
            destination = find_cup_by_label(current_cup, label);
        }
        printf("destination: %d\n", destination->label);

        // insert cups after destination
        selection->next->next->next = destination->next;
        destination->next = selection;

        current_cup = current_cup->next;
        printf("\n");
     }

    cup_t *c = find_cup_by_label(current_cup, 1)->next;
    for(size_t i=0; i < 8; i++, c = c->next) {
        printf("%d", c->label);
    }
}
