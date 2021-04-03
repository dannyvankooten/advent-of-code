#include <assert.h>
#include <err.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputs/21.h"

#define MAX_NAME_LENGTH 32

typedef struct {
  char (*ingredients)[MAX_NAME_LENGTH];
  size_t ningredients;
  char (*allergens)[MAX_NAME_LENGTH];
  size_t nallergens;
} food_t;

typedef struct {
  char* name;
  char** options;
  size_t noptions;
} allergen_t;

static void* emalloc(size_t size) {
  void* ptr = malloc(size);
  if (!ptr) {
    err(EXIT_FAILURE, "error allocating memory");
  }

  return ptr;
}

static void* erealloc(void* ptr, size_t size) {
  ptr = realloc(ptr, size);
  if (!ptr) {
    err(EXIT_FAILURE, "error allocating memory");
  }

  return ptr;
}

static size_t parse_input(food_t* dest) {
  const unsigned char *s = input;
  size_t n = 0;
  size_t i_cap = 16;
  size_t a_cap = 8;

  // mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
  while (*s != '\0') {
    food_t* f = &dest[n++];
    f->ningredients = 0;
    f->nallergens = 0;
    f->ingredients = emalloc(i_cap * MAX_NAME_LENGTH * sizeof(char));
    f->allergens = emalloc(a_cap * MAX_NAME_LENGTH * sizeof(char));

    while (*s != '(') {
      if (f->ningredients == i_cap) {
        i_cap *= 2;
        f->ingredients =
            erealloc(f->ingredients, i_cap * MAX_NAME_LENGTH * sizeof(char));
      }

      // patse ingredient name
      char* ingredient = f->ingredients[f->ningredients++];
      char* i = ingredient;
      while (*s != ' ') {
        *i++ = *s++;
      }
      *i = '\0';

      s++;  // skip ' '
    }
    
    s += strlen("(contains ");  // skip forward to first allergen

    while (*s != ')') {
      if (f->nallergens == a_cap) {
        a_cap *= 2;
        f->allergens =
            erealloc(f->allergens, a_cap * MAX_NAME_LENGTH * sizeof(char));
      }
      
      // parse allergen name
      char* allergen = f->allergens[f->nallergens++];
      char* a = allergen;
      while (*s != ',' && *s != ')') {
        *a++ = *s++;
      }
      *a = '\0';

      // skip , and space
      if (*s == ',' && *(s + 1) == ' ')
        s += 2;
    }

    s++; // ')'
    
    if (*s == '\n') s++;
  }

  return n;
}

void print_food(food_t* f) {
  for (size_t j = 0; j < f->ningredients; j++) {
    char* i = f->ingredients[j];
    printf("%s ", i);
  }

  if (f->nallergens > 0) {
    printf("(contains ");
    for (size_t j = 0; j < f->nallergens; j++) {
      if (j > 0) {
        printf(", ");
      }
      char* a = f->allergens[j];
      printf("%s", a);
    }
    printf(")");
  }
  printf("\n");
}

bool food_has_ingredient(food_t* f, char* ingredient) {
  for (size_t i = 0; i < f->ningredients; i++) {
    if (strcmp(f->ingredients[i], ingredient) == 0) {
      return true;
    }
  }

  return false;
}

allergen_t* get_allergen(allergen_t* list, size_t size, char* name) {
  for (size_t i = 0; i < size; i++) {
    if (strcmp(list[i].name, name) == 0) {
      return &list[i];
    }
  }

  return NULL;
}

// returns true if this ingredient is a possible option for any allergen
bool ingredient_can_contain_allergen(allergen_t* list,
                                     size_t size,
                                     char* ingredient) {
  for (size_t i = 0; i < size; i++) {
    allergen_t a = list[i];

    for (size_t j = 0; j < a.noptions; j++) {
      if (strcmp(a.options[j], ingredient) == 0) {
        return true;
      }
    }
  }

  return false;
}

int cmp_allergen(void const* p1, void const* p2) {
  allergen_t* a = (allergen_t*)p1;
  allergen_t* b = (allergen_t*)p2;
  return strcmp(a->name, b->name);
}

void remove_option_from_allergen(allergen_t* a, size_t index) {
  for (size_t i = index + 1; i < a->noptions; i++) {
    a->options[i - 1] = a->options[i];
  }
  a->noptions--;
}

int day21() {
  food_t* foods = emalloc(28 * sizeof(food_t));
  size_t nfoods = parse_input(foods);

  // go over each allergen, finding intersection of ingredients as we go
  size_t nallergens = 0;
  allergen_t* allergen_list = emalloc(28 * sizeof(allergen_t));
  for (size_t i = 0; i < nfoods; i++) {
    food_t* f = &foods[i];

    for (size_t j = 0; j < f->nallergens; j++) {
      allergen_t* a = get_allergen(allergen_list, nallergens, f->allergens[j]);
      if (a == NULL) {
        a = &allergen_list[nallergens++];
        a->name = f->allergens[j];
        a->options = emalloc(f->ningredients * sizeof(char*));
        a->noptions = f->ningredients;
        for (size_t k = 0; k < f->ningredients; k++) {
          a->options[k] = f->ingredients[k];
        }
      } else {
        // we've seen allergen before,
        // remove all options which are not an ingredient for current
        // food
        for (size_t k = 0; k < a->noptions; k++) {
          if (!food_has_ingredient(f, a->options[k])) {
            remove_option_from_allergen(a, k);
          }
        }
      }
    }
  }

  // // print options
  // for (size_t i=0; i < nallergens; i++) {
  //     printf("%s: ", allergen_list[i].name);
  //     for (size_t j=0; j < allergen_list[i].noptions; j++) {
  //         printf("%s, ", allergen_list[i].options[j]);
  //     }
  //     printf("\n");
  // }

  // then, find ingredients not in any of the allergen options
  int64_t count = 0;
  for (size_t i = 0; i < nfoods; i++) {
    food_t* f = &foods[i];
    for (size_t j = 0; j < f->ningredients; j++) {
      char* ingredient = f->ingredients[j];
      if (ingredient_can_contain_allergen(allergen_list, nallergens,
                                          ingredient)) {
        continue;
      }

      count++;
    }
  }
  printf("%" PRId64 "\n", count);
  assert(count == 1685);

  // find each allergen with only 1 option
  // remove this option from all other allergens, repeat until stable
  for (size_t i = 0; i < nallergens; i++) {
    allergen_t* a = &allergen_list[i];
    if (a->noptions != 1) {
      continue;
    }
    char* ingredient = a->options[0];
    bool stable = true;

    for (size_t j = 0; j < nallergens; j++) {
      // skip self
      if (j == i)
        continue;

      allergen_t* a2 = &allergen_list[j];
      for (size_t k = 0; k < a2->noptions; k++) {
        if (strcmp(a2->options[k], ingredient) == 0) {
          remove_option_from_allergen(a2, k);
          stable = false;
        }
      }
    }

    if (stable == false) {
      i = 0;
      continue;
    }
  }

  // sort alphabetically by allergen (so not ingredient name)
  qsort(allergen_list, nallergens, sizeof(allergen_t), cmp_allergen);

  // copy every option to answer string
  char answer[BUFSIZ] = { '\0' };
  char *s = answer;
  for (size_t i = 0; i < nallergens; i++) {
    if (i > 0) {
      *s++ = ',';
    }

    char *s2 = allergen_list[i].options[0];
    while (*s2 != '\0') {
      *s++ = *s2++;
    }
    *s = '\0';
  }
  printf("%s\n", answer);

  assert(strcmp(answer, "ntft,nhx,kfxr,xmhsbd,rrjb,xzhxj,chbtp,cqvc") == 0);

  // free foods, ingredients and allergens
  for (size_t i = 0; i < nallergens; i++) {
    free(allergen_list[i].options);
  }
  free(allergen_list);
  for (size_t i = 0; i < nfoods; i++) {
    free(foods[i].allergens);
    free(foods[i].ingredients);
  }
  free(foods);
  return 0;
}
