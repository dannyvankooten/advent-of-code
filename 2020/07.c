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

static
void parse_rules_from_input() {
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
    const char* s = linebuf;
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
    const char* pos = strstr(s, " bags contain ");
    uint8_t len = pos - s;
    memcpy(bag->color, s, len);
    bag->color[len] = '\0';
    s = pos;


    // skip " bags contain "
    s += strlen(" bags contain ");

    // parse all children
    while (1) {

      // skip whitespace
      while (*s == ' ') {
        s++;
      }

      // parse child quantity
      qty = 0;
      while (*s >= '0' && *s <= '9') {
        qty = qty * 10 + *s - '0';
        s++;
      }

      bag->children[bag->nchildren].qty = qty;

      // skip whitespace
      while (*s == ' ') {
        s++;
      }

      // parse child color
      pos = strstr(s, " bag");
      len = pos - s;
      memcpy(bag->children[bag->nchildren].color, s, len);
      bag->children[bag->nchildren].color[len] = '\0';
      bag->nchildren++;
      s = pos;

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

static
bag_t* find_bag(const char *color) {
  bag_t* b;
  b = (bag_t*)ht_get(bags, color);
  return b;
}

static bool
may_bag_contain_color(bag_t* b, const char *color) {
  if (b == NULL) {
    return false;
  }

  // search children for the given color
  for (int32_t j = 0; j < b->nchildren; j++) {
    if (strcmp(b->children[j].color, color) == 0) {
      return true;
    }

    // search children of this bag
    if (may_bag_contain_color(find_bag(b->children[j].color), color)) {
      return true;
    }
  }

  return false;
}

static
int32_t search_bags_for_color(const char *color) {
  int32_t count = 0;
  hti it = ht_iterator(bags);
  while (ht_next(&it)) {
    count += may_bag_contain_color((bag_t*) it.value, color);
  }
  return count;
}

static
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

  int32_t part_1 = search_bags_for_color("shiny gold");
  assert(part_1 == 211);
  printf("%d\n", part_1);
  int32_t part_2 = count_children(shiny_gold);
  printf("%d\n", part_2);
  assert(part_2 == 12414);

  free(mbags);
  ht_destroy(bags);
  return 0;
}
