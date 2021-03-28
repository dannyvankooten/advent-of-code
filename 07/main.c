#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

struct Bag {
    char color[32];
    int nchildren;
    struct {
        char color[32];
        int qty;
    } children[16];
};

struct Bags {
    struct Bag *values;
    int size;
    int cap;
};

struct Bags
parse_rules_from_input(char *input_file) {
    struct Bags bags = {
        .values = malloc(512 *sizeof(struct Bag)),
        .size = 0,
        .cap = 512,
    };
    int i;
    int qty;

    FILE *f = fopen(input_file, "r");
    if (!f) {
        err(EXIT_FAILURE, "could not open input file");
    }
    char linebuf[BUFSIZ] = {0};

    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        char *str = linebuf;
    
        struct Bag bag = {
            .nchildren = 0,
            .color = "",
        };
        
        // parse up to " contain "
        char *pos = strstr(str, " bags contain ");
        for (i=0; str < pos; i++, str++) {
            bag.color[i] = *str;
        }
        bag.color[i] = '\0';
        
        // skip " bags contain "
        str += strlen(" bags contain ");

        // parse all children
        while (*str != '.') {
            while (*str == ' ') str++;

            // parse child quantity
            qty = 0;
            while (*str >= '0' && *str <= '9') {
                qty = qty * 10 + *str - '0';
                str++;
            }
            
            bag.children[bag.nchildren].qty = qty;

            // skip whitespace
            while (*str == ' ') str++;
            
            // parse child color
            pos = strstr(str, " bag");
            for (i=0; str < pos; str++) {
                bag.children[bag.nchildren].color[i++] = *str;
            }
            bag.children[bag.nchildren].color[i] = '\0';
            bag.nchildren++;

            // skip forward to after next comma or dot
            while (*(str-1) != ',' && *(str-1) != '.') {
                str++;
            }

            // if it was a dot, we're done parsing children
            if (*(str-1) == '.') {
                break;
            }
        }

        // Add bag to list
        if (bags.size == bags.cap) {
            bags.cap *= 2;
            bags.values = realloc(bags.values, bags.cap * sizeof(struct Bag));
        }
        bags.values[bags.size++] = bag; 
    }

    fclose(f);
    return bags;
}

void
print_bags(struct Bags bags) 
{
    printf("%d bags, capacity %d\n", bags.size, bags.cap);
    for (int i=0; i < bags.size; i++) {
        printf("%s", bags.values[i].color);

        if (bags.values[i].nchildren > 0) {
            printf(" contains ");

            for (int j=0; j < bags.values[i].nchildren; j++) {
                if (j > 0) {
                    printf(", ");
                }
                printf("%d %s", bags.values[i].children[j].qty, bags.values[i].children[j].color);
            }
        }

        printf("\n");
    }
}

struct Bags bags;

struct Bag *
find_bag(char *color) {
    struct Bag *b;
    for (int i=0; i < bags.size; i++) {
       b = &bags.values[i];
       if (strcmp(b->color, color) == 0) {
           return b;
       }
    }

    return NULL;
}

int may_bag_contain_color(struct Bag *b, char *color) {
    if (b == NULL) {
        return 0;
    }

    // search children for the given color
    for (int j=0; j < b->nchildren; j++) {
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

int search_bags_for_color(char *color) {
    int count = 0;

    for (int i=0; i < bags.size; i++) {
        count += may_bag_contain_color(&bags.values[i], color);
    }

    return count;
}

int count_children(struct Bag *b)
{
    if (b == NULL) {
        return 0;
    }

    int count = 0;
    for (int i=0; i < b->nchildren; i++) {
        count += (1 + count_children(find_bag(b->children[i].color))) * b->children[i].qty;
    }

    return count;
}

int main() 
{
    // test input
    // bags = parse_rules("light red bags contain 1 bright white bag, 2 muted yellow bags.\n"
    //     "dark orange bags contain 3 bright white bags, 4 muted yellow bags.\n"
    //     "bright white bags contain 1 shiny gold bag.\n"
    //     "muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.\n"
    //     "shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.\n"
    //     "dark olive bags contain 3 faded blue bags, 4 dotted black bags.\n"
    //     "vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.\n"
    //     "faded blue bags contain no other bags.\n"
    //     "dotted black bags contain no other bags.\n");

    // print_bags(bags);
    // printf("%d bags can contain shiny gold\n", search_bags_for_color("shiny gold"));
    // printf("%d bags can contain dotted black\n", search_bags_for_color("dotted black"));
    // printf("%d bags can contain dark orange\n", search_bags_for_color("dark orange"));

    bags = parse_rules_from_input("input.txt");


    printf("%d bags can contain shiny gold\n", search_bags_for_color("shiny gold"));

    struct Bag *shiny_gold = find_bag("shiny gold");
    assert(shiny_gold != NULL);
    printf("children of shiny bag: %d\n", count_children(shiny_gold));  

    free(bags.values); 
}
