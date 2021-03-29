#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#define MAX_NAME_LENGTH 32

typedef struct {
    char (*ingredients)[MAX_NAME_LENGTH];
    size_t ningredients;
    char (*allergens)[MAX_NAME_LENGTH];
    size_t nallergens;
} food_t; 

typedef struct {
    char name[MAX_NAME_LENGTH];
    char **options;
    size_t noptions;
} allergen_t;

size_t  
parse_input(food_t *dest) {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        err(EXIT_FAILURE, "error reading input file");
    }

    size_t n = 0;
    size_t i_cap = 16;
    size_t a_cap = 8;

    char linebuf[BUFSIZ] = {0};
    while (fgets(linebuf, BUFSIZ, fp) != NULL) {
        food_t *f = &dest[n++];
        f->ningredients = 0;
        f->nallergens = 0;
        f->ingredients = malloc(i_cap * sizeof(char[MAX_NAME_LENGTH]));
        f->allergens = malloc(a_cap * sizeof(char[MAX_NAME_LENGTH]));
        
        char *s = linebuf;
        while (*s != '(') {
            if (f->ningredients == i_cap) {
                i_cap *= 2;
                f->ingredients = realloc(f->ingredients, i_cap * sizeof(char[MAX_NAME_LENGTH]));
            }

            char *ingredient = f->ingredients[f->ningredients++];
            char *i = ingredient;
            while (*s != ' ') {
                *i++ = *s++;
            }
            *i = '\0';

            s++; // skip space
        }

        if (*s == '(') {
            s += 10; // skip forward to first allergen

            while (*s != ')') {
                if (f->nallergens == a_cap) {
                    a_cap *= 2;
                    f->allergens = realloc(f->allergens, a_cap  * sizeof(char[MAX_NAME_LENGTH]));
                }
                char *allergen = f->allergens[f->nallergens++];
                char *a = allergen;
                while (*s != ',' && *s != ')') {
                    *a++ = *s++;
                }
                *a = '\0';

                // skip , and space
                if (*s == ',') s += 2;
            }
        }
       
    }
    fclose(fp);
    return n;
}

void 
print_food(food_t *f) {
    for (size_t j = 0; j < f->ningredients; j++) {
        char *i = f->ingredients[j];
        printf("%s ", i);
    }

    if (f->nallergens > 0) {
        printf("(contains ");
        for (size_t j = 0; j < f->nallergens; j++) {
            if (j > 0) {
                printf(", ");
            }
            char *a = f->allergens[j];
            printf("%s", a);
        }
        printf(")");
    }
    printf("\n");
}

bool 
food_has_ingredient(food_t *f, char *ingredient) {
    for (size_t i=0; i < f->ningredients; i++) {
        if (strcmp(f->ingredients[i], ingredient) == 0) {
            return true;
        }
    }

    return false;
}


bool 
food_has_allergen(food_t *f, char *allergen) {
    for (size_t i=0; i < f->nallergens; i++) {
        if (strcmp(f->allergens[i], allergen) == 0) {
            return true;
        }
    }

    return false;
}

bool 
food_cmp_allergens(food_t *f1, food_t *f2) {
    for (size_t i=0; i < f1->nallergens; i++) {
        for (size_t j=0; j < f2->nallergens; j++) {
            if (strcmp(f1->allergens[i], f2->allergens[j]) == 0) {
                return true;
            }
        }
    }

    return false;
}

allergen_t *
get_allergen(allergen_t *list, size_t size, char *name) {
    for (size_t i=0; i < size; i++) {
        if (strcmp(list[i].name, name) == 0) {
            return &list[i];
        }
    }

    return NULL;
}

bool 
ingredient_can_contain_allergen(allergen_t *list, size_t size, char *ingredient) {
    for (size_t i=0; i < size; i++) {
        allergen_t a = list[i];

        for (size_t j=0; j < a.noptions; j++) {
            if (a.options[j] == NULL) {
                continue;
            }

            if (strcmp(a.options[j], ingredient) == 0) {
                return true;
            }
        }
    }

    return false;
}



int cmp_allergen(void const *p1, void const *p2) { 
    allergen_t *a = (allergen_t *) p1;
    allergen_t *b = (allergen_t *) p2;

    return strcmp(a->name, b->name);
}

int main() {
    food_t * foods = malloc(28 * sizeof(food_t));
    size_t nfoods = parse_input(foods);

    // go over each allergen, finding intersection of ingredients as we go
    size_t nallergens = 0;
    allergen_t *allergen_list = malloc(28 * sizeof(allergen_t));
    for (size_t i = 0; i < nfoods; i++) {
        food_t *f = &foods[i];
        
        for (size_t j=0; j < f->nallergens; j++) {
            allergen_t *a = get_allergen(allergen_list, nallergens, f->allergens[j]);
            if (a == NULL) {
                a = &allergen_list[nallergens++];
                strcpy(a->name, f->allergens[j]);
                a->options = malloc(f->ningredients * sizeof(char *));
                a->noptions = f->ningredients;
                for (size_t k=0; k < f->ningredients; k++) {
                    a->options[k] = f->ingredients[k];
                }
            } else {
                // we've seen allergen before, 
                // remove all options which are not an ingredient for current food
                for (size_t k=0; k < a->noptions; k++) {
                    if (!a->options[k]) continue;
                    if (!food_has_ingredient(f, a->options[k])) {
                        a->options[k] = NULL;
                    }
                }
            }
        }
    }

    // print options
    for (size_t i=0; i < nallergens; i++) {
        printf("%s: ", allergen_list[i].name);
        for (size_t j=0; j < allergen_list[i].noptions; j++) {
            if (allergen_list[i].options[j]) {
                printf("%s, ", allergen_list[i].options[j]);
            }
        }
        printf("\n");
    }

    // then, find ingredients not in any of the allergen options
    int64_t count = 0;
    for (size_t i = 0; i < nfoods; i++) {
        food_t *f = &foods[i];
        for (size_t j=0; j < f->ningredients; j++) {
            char *ingredient = f->ingredients[j];
            if (ingredient_can_contain_allergen(allergen_list, nallergens, ingredient)) {
                continue;
            }

            count++;
        }
     }
     printf("Part 1: %" PRId64  "\n", count);

    // find each allergen with only 1 option
    // remove this option from all other allergens, repeat until stable
    for (size_t i=0; i < nallergens; i++) {
        size_t noptions = 0;
        size_t last_idx = 0;
        for (size_t j=0; j < allergen_list[i].noptions; j++) {
            if (!allergen_list[i].options[j]) continue;
            noptions++;
            last_idx = j;            
        }
        if (noptions == 1) {
            char *ingredient = allergen_list[i].options[last_idx];
            bool stable = true;
            for (size_t k=0; k < nallergens; k++) {
                if (k == i) { continue; }

                for (size_t l=0; l < allergen_list[k].noptions; l++) {                    
                    if (allergen_list[k].options[l] != NULL && strcmp(allergen_list[k].options[l], ingredient) == 0) {
                        allergen_list[k].options[l] = NULL;
                        stable = false;
                    }
                }
            }

            if (!stable) {
                i = 0;
                continue;
            }

        }
    }
    
    qsort(allergen_list, nallergens, sizeof(allergen_t), cmp_allergen);
    printf("Part 2: ");
    for (size_t i=0; i < nallergens; i++) {
       if (i > 0) printf(",");
       for (size_t j=0; j < allergen_list[i].noptions; j++) {
           if (allergen_list[i].options[j] == NULL) continue;
           printf("%s", allergen_list[i].options[j]);
           break;
       }
    }
    printf("\n");


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
}
