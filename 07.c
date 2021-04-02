#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "ht.h"

struct Bag {
    char color[32];
    int32_t nchildren;
    struct {
        char color[32];
        int32_t qty;
    } children[16];
};

ht* bags;
struct Bag *mbags;

void
parse_rules_from_input(char *input_file) {
    int32_t i;
    int32_t qty;

    int32_t cap = 1024;
    int32_t size = 0;
    mbags = (struct Bag *) malloc(cap * sizeof(struct Bag));

    FILE *f = fopen(input_file, "r");
    if (!f) {
        err(EXIT_FAILURE, "could not open input file");
    }
    char linebuf[BUFSIZ] = {0};
    bags = ht_create();
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        char *str = linebuf;
        struct Bag *bag = &mbags[size++];
        if (size == cap) {
            cap *= 2;
            mbags = (struct Bag *) realloc(mbags, cap * sizeof(struct Bag));
        }
        //struct Bag *bag = malloc(sizeof(struct Bag));
        bag->nchildren = 0;
        bag->color[0] = '\0';
        
        // parse up to " contain "
        char *pos = strstr(str, " bags contain ");
        for (i=0; str < pos;) {
            bag->color[i++] = *str++;
        }
        bag->color[i] = '\0';
        
        // skip " bags contain "
        str += strlen(" bags contain ");

        // parse all children
        while (1) {
            while (*str == ' ') str++;

            // parse child quantity
            qty = 0;
            while (*str >= '0' && *str <= '9') {
                qty = qty * 10 + *str - '0';
                str++;
            }
            
            bag->children[bag->nchildren].qty = qty;

            // skip whitespace
            while (*str == ' ') str++;
            
            // parse child color
            pos = strstr(str, " bag");
            for (i=0; str < pos;) {
                bag->children[bag->nchildren].color[i++] = *str++;
            }
            bag->children[bag->nchildren].color[i] = '\0';
            bag->nchildren++;

            // skip forward to after next comma or dot
            while (*(str) != ',' && *str != '.') {
                str++;
            }

            // if it was a dot, we're done parsing children
            if (*str == '.') {
                break;
            } else {
                str++; // skip ,
            }
        }

        if (ht_set(bags, bag->color, (void *) bag) == NULL) {
            err(EXIT_FAILURE, "error adding to hashmap");
        } 
    }

    fclose(f);
}

struct Bag *
find_bag(const char color[32]) {
    struct Bag *b;
    b = (struct Bag *) ht_get(bags, color);
    return b;
}

int32_t may_bag_contain_color(struct Bag *b, const char color[32]) {
    if (b == NULL) {
        return 0;
    }

    // search children for the given color
    for (int32_t j=0; j < b->nchildren; j++) {
        if (strcmp(b->children[j].color, color) == 0) {
            return 1;
        }

        // search children of this bag
        if (1 == may_bag_contain_color(find_bag(b->children[j].color), color)) {
            return 1;
        }
    }

    return 0;
}

int32_t search_bags_for_color(const char color[32]) {
    int32_t count = 0;
    hti it = ht_iterator(bags);
    while (ht_next(&it)) {
        count += may_bag_contain_color(it.value, color);
    }
    return count;
}

int32_t count_children(struct Bag *b)
{
    if (b == NULL) {
        return 0;
    }

    int32_t count = 0;
    for (int32_t i=0; i < b->nchildren; i++) {
        count += (1 + count_children(find_bag(b->children[i].color))) * b->children[i].qty;
    }

    return count;
}

int day7() 
{
    parse_rules_from_input("07.input");
    struct Bag *shiny_gold = find_bag("shiny gold");
    assert(shiny_gold != NULL);

    printf("%d\n", search_bags_for_color("shiny gold"));
    printf("%d\n", count_children(shiny_gold));  

    free(mbags);
    ht_destroy(bags);
    return 0;
}
