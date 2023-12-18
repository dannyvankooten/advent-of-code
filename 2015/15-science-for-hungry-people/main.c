#include "../adventofcode.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

struct ingredient {
  int capacity;
  int durability;
  int flavor;
  int texture;
  int calories;
};

static const char *skip_until_number(const char *s) {
  while ((*s < '0' || *s > '9') && *s != '-') {
    s++;
  }
  return s;
}

void parse(struct ingredient *ingredients, int *ningredients) {
  char input[64 * 1024];
  read_input_file(input, 64 * 1024, "input.txt");

  int n = 0;
  const char *s = input;
  while (*s != '\0') {
    // skip name to save 14 bytes
    // s = parse_ident(ingredients[n].name, s);
    s = skip_until_number(s);
    s = parse_int(&ingredients[n].capacity, s);
    s = skip_until_number(s);
    s = parse_int(&ingredients[n].durability, s);
    s = skip_until_number(s);
    s = parse_int(&ingredients[n].flavor, s);
    s = skip_until_number(s);
    s = parse_int(&ingredients[n].texture, s);
    s = skip_until_number(s);
    s = parse_int(&ingredients[n].calories, s);
    s = skip_until('\n', s);
    s = skip_optional('\n', s);
    n++;
  }

  *ningredients = n;
}

int main() {
  clock_t start_t = clock_time();

  struct ingredient ingredients[4];
  int ningredients = 0;
  parse(ingredients, &ningredients);

  int capacity;
  int durability;
  int flavor;
  int texture;
  int score;
  int calories;
  int max_score = -1;
  int max_score_500c = -1;
  for (int i = 0; i < 100; i++) {
    for (int k = i; k < 100 - i; k++) {
      for (int l = k; l < 100 - k; l++) {
        int m = 100 - i - k - l;

        capacity = ingredients[0].capacity * i + ingredients[1].capacity * k +
                   ingredients[2].capacity * l + ingredients[3].capacity * m;
        if (capacity <= 0)
          continue;

        durability =
            ingredients[0].durability * i + ingredients[1].durability * k +
            ingredients[2].durability * l + ingredients[3].durability * m;
        if (durability <= 0)
          continue;

        flavor = ingredients[0].flavor * i + ingredients[1].flavor * k +
                 ingredients[2].flavor * l + ingredients[3].flavor * m;
        if (flavor <= 0)
          continue;

        texture = ingredients[0].texture * i + ingredients[1].texture * k +
                  ingredients[2].texture * l + ingredients[3].texture * m;
        if (texture <= 0)
          continue;

        score = capacity * durability * flavor * texture;
        if (score > max_score) {
          max_score = score;
        }

        calories = ingredients[0].calories * i + ingredients[1].calories * k +
                   ingredients[2].calories * l + ingredients[3].calories * m;
        if (calories == 500 && score > max_score_500c) {
          max_score_500c = score;
        }
      }
    }
  }

  printf("--- Day 15: Science for Hungry People ---\n");
  printf("Part 1: %d %s\n", max_score, max_score == 18965440 ? "✔" : "");
  printf("Part 2: %d %s\n", max_score_500c,
         max_score_500c == 15862900 ? "✔" : "");
  printf("Time: %.2fms\n", clock_time_since(start_t));
  return 0;
}