#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct stats {
  int hitpoints;
  int damage;
  int armor;
};

struct equipment {
  int cost;
  int damage;
  int armor;
};

struct stats parse(const char *s) {
  struct stats boss;

  s = skip_until_digit(s);
  s = parse_int(&boss.hitpoints, s);
  s = skip_until_digit(s);
  s = parse_int(&boss.damage, s);
  s = skip_until_digit(s);
  s = parse_int(&boss.armor, s);

  return boss;
}

struct equipment weapons[] = {
    {8, 4, 0}, {10, 5, 0}, {25, 6, 0}, {40, 7, 0}, {74, 8, 0},
};
struct equipment armor[] = {
    {0, 0, 0}, {13, 0, 1}, {31, 0, 2}, {53, 0, 3}, {75, 0, 4}, {102, 0, 5},
};
struct equipment rings[] = {
    {0, 0, 0},  {25, 1, 0}, {50, 2, 0}, {100, 3, 0},
    {20, 0, 1}, {40, 0, 2}, {80, 0, 3},
};

int damage(int attacker, int defender) {
  int diff = attacker - defender;
  return diff < 1 ? 1 : diff;
}

int fight(struct stats player, struct stats boss) {
  while (player.hitpoints > 0) {
    boss.hitpoints -= damage(player.damage, boss.armor);
    if (boss.hitpoints <= 0) {
      return 1; // player wins
    }

    player.hitpoints -= damage(boss.damage, player.armor);
  }

  return 0;
}

struct answer {
  int pt1;
  int pt2;
};

struct answer permute(struct stats player, struct stats boss) {
  int cheap_win = 1 << 30;
  int expensive_lose = -1;
  for (int w = 0; w < 5; w++) {
    for (int a = 0; a < 6; a++) {
      for (int r1 = 0; r1 < 7; r1++) {
        for (int r2 = 0; r2 < 7; r2++) {
          if (r1 == r2) {
            continue;
          }
          player.hitpoints = 100;
          player.damage =
              weapons[w].damage + rings[r1].damage + rings[r2].damage;
          player.armor = armor[a].armor + rings[r1].armor + rings[r2].armor;
          int cost =
              weapons[w].cost + armor[a].cost + rings[r1].cost + rings[r2].cost;
          int wins = fight(player, boss) == 1;

          if (wins && cost < cheap_win) {
            cheap_win = cost;
          }

          if (wins == 0 && cost > expensive_lose) {
            expensive_lose = cost;
          }
        }
      }
    }
  }

  return (struct answer){cheap_win, expensive_lose};
}

int main() {
  clock_t start_t = clock_time();
  char input[1024];
  read_input_file(input, 1024, "input.txt");

  struct stats player = {100, 0, 0};
  struct stats boss = parse(input);
  struct answer a = permute(player, boss);

  printf("--- Day 21: RPG Simulator 20XX ---\n");
  printf("Part 1: %d\n", a.pt1);
  printf("Part 2: %d\n", a.pt2);
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}

// 113 too high
