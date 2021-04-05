#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ht.h"

struct bag {
  uint8_t nchildren;
  char color[24];
  struct {
    char color[24];
    uint8_t qty;
  } children[6];
};
typedef struct bag bag_t;

ht* bags;
bag_t* mbags;

void parse_rules_from_input() {
  int32_t i;
  int32_t qty;
  int32_t cap = 1024;
  int32_t size = 0;
  mbags = (bag_t*)malloc(cap * sizeof(bag_t));
  if (!mbags) {
    err(EXIT_FAILURE, "error allocating memory for bags");
  }

  FILE* f = fopen("inputs/07.txt", "r");
  if (!f) {
    err(EXIT_FAILURE, "could not open input file");
  }
  char linebuf[BUFSIZ] = {0};
  bags = ht_create();
  while (fgets(linebuf, BUFSIZ, f) != NULL) {
    char* s = linebuf;
    bag_t* bag = &mbags[size++];
    if (size == cap) {
      cap *= 2;
      mbags = (bag_t*)realloc(mbags, cap * sizeof(bag_t));
      if (!mbags) {
        err(EXIT_FAILURE, "error allocating memory for bags");
      }
    }
    // bag_t *bag = malloc(sizeof(bag_t));
    bag->nchildren = 0;
    bag->color[0] = '\0';

    // parse up to " contain "
    char* pos = strstr(s, " bags contain ");
    for (i = 0; s < pos;) {
      bag->color[i++] = *s++;
    }
    bag->color[i] = '\0';

    // skip " bags contain "
    s += strlen(" bags contain ");

    // parse all children
    while (1) {
      while (*s == ' ')
        s++;

      // parse child quantity
      qty = 0;
      while (*s >= '0' && *s <= '9') {
        qty = qty * 10 + *s - '0';
        s++;
      }

      bag->children[bag->nchildren].qty = qty;

      // skip whitespace
      while (*s == ' ')
        s++;

      // parse child color
      pos = strstr(s, " bag");
      for (i = 0; s < pos;) {
        bag->children[bag->nchildren].color[i++] = *s++;
      }
      bag->children[bag->nchildren].color[i] = '\0';
      bag->nchildren++;

      // skip forward to after next comma or dot
      while (*s != ',' && *s != '.') {
        s++;
      }

      // if it was a dot, we're done parsing children
      if (*s == '.') {
        break;
      } else {
        s++;  // skip ,
      }
    }

    if (ht_set(bags, bag->color, (void*)bag) == NULL) {
      err(EXIT_FAILURE, "error adding to hashmap");
    }
  }

  fclose(f);
}

bag_t* find_bag(const char color[32]) {
  bag_t* b;
  b = (bag_t*)ht_get(bags, color);
  return b;
}

int32_t may_bag_contain_color(bag_t* b, const char color[32]) {
  if (b == NULL) {
    return 0;
  }

  // search children for the given color
  for (int32_t j = 0; j < b->nchildren; j++) {
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

int32_t count_children(bag_t* b) {
  if (b == NULL) {
    return 0;
  }

  int32_t count = 0;
  for (int32_t i = 0; i < b->nchildren; i++) {
    count += (1 + count_children(find_bag(b->children[i].color))) *
             b->children[i].qty;
  }

  return count;
}

int day7() {
  parse_rules_from_input();
  bag_t* shiny_gold = find_bag("shiny gold");
  assert(shiny_gold != NULL);

  printf("%d\n", search_bags_for_color("shiny gold"));
  printf("%d\n", count_children(shiny_gold));

  free(mbags);
  ht_destroy(bags);
  return 0;
}
