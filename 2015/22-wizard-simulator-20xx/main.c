#include "../adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct stats {
  int hitpoints;
  int damage;
};

struct spell {
  int mana_cost;
  int damage;
  int heal;
  int time_effect;
  int damage_effect;
  int armor_effect;
  int mana_effect;
};

struct stats parse(const char *s) {
  struct stats boss;

  s = skip_until_digit(s);
  s = parse_int(&boss.hitpoints, s);
  s = skip_until_digit(s);
  s = parse_int(&boss.damage, s);

  return boss;
}

enum {
  SPELL_MAGIC_MISSILE = 0,
  SPELL_DRAIN,
  SPELL_SHIELD,
  SPELL_POISON,
  SPELL_RECHARGE,
};

struct spell spells[] = {
    {53, 4, 0, 0, 0, 0, 0},    // magic missile: 4 damage
    {73, 2, 2, 0, 0, 0, 0},    // drain: 2 damage, 2 heal
    {113, 0, 0, 6, 0, 7, 0},   // shield: +7 armor for 6 turns
    {173, 0, 0, 6, 3, 0, 0},   // poison: +3 damage for 6 turns
    {229, 0, 0, 5, 0, 0, 101}, // recharge: +101 mana for 5 turns
};

int damage(int attacker, int defender) {
  int diff = attacker - defender;
  return diff < 1 ? 1 : diff;
}

enum {
  PLAYER,
  BOSS,
};

int fight(struct stats boss) {
  char turn = PLAYER; // player starts
  int timers[] = {0, 0, 0, 0, 0};
  int player_mana = 250;
  int player_hitpoints = 10;

  for (int round = 0; player_hitpoints > 0; round++) {
    int instant_damage = 0;
    int damage_effect = 0;
    int player_armor = 0;

    // apply running spells and decrease timer
    for (int t = 0; t < 5; t++) {
      if (timers[t] > 0) {
        timers[t]--;
        player_mana += spells[t].mana_effect;
        damage_effect += spells[t].damage_effect;
        player_armor += spells[t].armor_effect;
        player_hitpoints += spells[t].heal;
      }
    }

    // deal effect damage
    if (damage_effect > 0) {
      // printf("Poison deals %d damage. Its timer is now %d.\n", damage_effect,
      //        timers[SPELL_POISON]);
      boss.hitpoints -= damage_effect;
    }
    if (boss.hitpoints <= 0) {
      return 1; // player wins
    }

    // printf("-- %s turn --\n", turn == PLAYER ? "Player" : " Boss");
    // printf("- Player has %d hitpoints, %d armor, %d mana\n",
    // player_hitpoints,
    //        player_armor, player_mana);
    // printf("- Boss has %d hit points\n", boss.hitpoints);

    // deal instant damages
    if (turn == PLAYER) {

      // we have at most 5 options here
      // recurse into every possible option and return the amount of mana spent
      // after winning a game
      if (round == 0) {
        // apply spell
        timers[SPELL_POISON] = spells[SPELL_POISON].time_effect;
        player_mana -= spells[SPELL_POISON].mana_cost;
      } else {
        // cast new spell
        player_mana -= spells[0].mana_cost;
        instant_damage += spells[0].damage;
        boss.hitpoints -= instant_damage;
        // printf("Player deals %d damage\n", instant_damage);
      }
    } else {
      instant_damage =
          (boss.damage - player_armor) > 1 ? boss.damage - player_armor : 1;
      player_hitpoints -= instant_damage;
      // printf("Boss deals %d damage\n", instant_damage);
    }

    if (boss.hitpoints <= 0) {
      return 1; // player wins
    }

    printf("\n");

    turn = !turn;
  }

  return 0;
}

struct answer {
  int pt1;
  int pt2;
};

// struct answer permute(struct stats boss) {
//   int cheap_win = 1 << 30;
//   int expensive_lose = -1;

//   for (unsigned int s = 1; s < sizeof(spells) / sizeof(spells[0]); s++) {
//   }

//   return (struct answer){cheap_win, expensive_lose};
// }

int main() {
  clock_t start_t = clock_time();
  char input[1024];
  read_input_file(input, 1024, "input_test.txt");

  struct stats boss = parse(input);
  struct answer a = {0, 0};

  printf("%d\n", fight(boss));

  printf("--- Day 22: Wizard Simulator 20XX ---\n");
  printf("Part 1: %d\n", a.pt1);
  printf("Part 2: %d\n", a.pt2);
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}

// 113 too high
