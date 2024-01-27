#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static inline char *parse_int(int_fast8_t *dst, char *s) {
  int_fast16_t mod = 1;
  int_fast8_t n = 0;

  if (*s == '-') {
    mod = -1;
    s++;
  }

  while (*s >= '0' && *s <= '9') {
    n = (n * 10) + (*s - '0');
    s++;
  }
  *dst = n * mod;
  return s;
}

static inline char *parse_ident(char *dst, char *s) {
  while (*s != ':') {
    *dst++ = *s++;
  }
  *dst++ = '\0';
  return s;
}

typedef struct ingredient {
  char name[14];
  int capacity;
  int durability;
  int flavor;
  int texture;
  int calories;
} ingredient_t;

void parse(int_fast8_t capacities[], int_fast8_t durabilities[],
           int_fast8_t flavors[], int_fast8_t textures[],
           int_fast8_t calories[], int *ningredients) {
  FILE *fp = fopen("input.txt", "r");
  char input[64 * 1024];
  size_t nread = fread(input, 1, 1024 * 32, fp);
  fclose(fp);
  input[nread] = '\0';

  int n = 0;
  char *s = input;
  while (*s != '\0') {
    while ((*s < '0' || *s > '9') && *s != '-')
      s++;
    s = parse_int(&capacities[n], s);
    while ((*s < '0' || *s > '9') && *s != '-')
      s++;
    s = parse_int(&durabilities[n], s);
    while ((*s < '0' || *s > '9') && *s != '-')
      s++;
    s = parse_int(&flavors[n], s);
    while ((*s < '0' || *s > '9') && *s != '-')
      s++;
    s = parse_int(&textures[n], s);
    while ((*s < '0' || *s > '9') && *s != '-')
      s++;
    s = parse_int(&calories[n], s);

    while (*s != '\n' && *s != '\0')
      s++;
    if (*s == '\n')
      s++;

    n++;
  }

  *ningredients = n;
}

int main() {
  clock_t start_t, end_t;
  start_t = clock();

  int ningredients = 0;
  int_fast8_t *capacities = (int_fast8_t *)malloc(20 * sizeof(int_fast8_t));
  int_fast8_t *durabilities = &capacities[4];
  int_fast8_t *flavors = &capacities[8];
  int_fast8_t *textures = &capacities[12];
  int_fast8_t *calories = &capacities[16];
  parse(capacities, durabilities, flavors, textures, calories, &ningredients);

  int capacity;
  int durability;
  int flavor;
  int texture;
  int score;
  int total_calories;
  int max_score = -1;
  int max_score_500c = -1;
  for (int i = 0; i < 100; i++) {
    for (int k = 0; k < 100; k++) {
      for (int l = 0; l < 100; l++) {
        for (int m = 0; m < 100; m++) {
          if (i + k + l + m != 100)
            continue;

          capacity = capacities[0] * i + capacities[1] * k + capacities[2] * l +
                     capacities[3] * m;
          if (capacity <= 0)
            continue;

          durability = durabilities[0] * i + durabilities[1] * k +
                       durabilities[2] * l + durabilities[3] * m;
          if (durability <= 0)
            continue;

          flavor =
              flavors[0] * i + flavors[1] * k + flavors[2] * l + flavors[3] * m;
          if (flavor <= 0)
            continue;

          texture = textures[0] * i + textures[1] * k + textures[2] * l +
                    textures[3] * m;
          if (texture <= 0)
            continue;

          score = capacity * durability * flavor * texture;
          if (score > max_score) {
            max_score = score;
          }

          total_calories = calories[0] * i + calories[1] * k + calories[2] * l +
                           calories[3] * m;
          if (total_calories == 500 && score > max_score_500c) {
            max_score_500c = score;
          }
        }
      }
    }
  }

  printf("part 1: %d\n", max_score);
  printf("part 2: %d\n", max_score_500c);
  assert(max_score == 18965440);

  assert(max_score_500c == 15862900);

  end_t = clock();
  double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
  printf("%.2fms\n", total_t);
  return 0;
}