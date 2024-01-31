#include "../adventofcode.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct boss_stats {
  int hitpoints;
  int damage;
};

struct player_stats {
  int hitpoints;
  unsigned int mana;
  unsigned int mana_spent;
};

struct spell {
  unsigned int mana_cost;
  int damage;
  int heal;
  unsigned char time_effect;
  int damage_effect;
  int armor_effect;
  unsigned int mana_effect;
};

static struct boss_stats parse(const char *s) {
  struct boss_stats boss;

  s = skip_until_digit(s);
  s = parse_int(&boss.hitpoints, s);
  s = skip_until_digit(s);
  s = parse_int(&boss.damage, s);

  return boss;
}

enum {
  PLAYER = 0,
  BOSS,
};

static const struct spell spells[] = {
    {53, 4, 0, 0, 0, 0, 0},    // magic missile: 4 instant-damage
    {73, 2, 2, 0, 0, 0, 0},    // drain: 2 instant-damage, 2 instant-heal
    {113, 0, 0, 6, 0, 7, 0},   // shield: +7 armor for 6 turns
    {173, 0, 0, 6, 3, 0, 0},   // poison: +3 damage for 6 turns
    {229, 0, 0, 5, 0, 0, 101}, // recharge: +101 mana for 5 turns
};

static unsigned int fight(struct boss_stats boss, struct player_stats player,
                          unsigned char timers[5], const char turn,
                          unsigned int least_mana_spent,
                          const int hitpoints_drain) {
  if (player.hitpoints <= 0) {
    return INT_MAX;
  }

  int damage_effect = 0;
  int player_armor = 0;

  // apply running spell effects and decrease timer
  for (unsigned s = 0; s < 5; s++) {
    if (timers[s] == 0) {
      continue;
    }

    timers[s]--;
    player.mana += spells[s].mana_effect;
    damage_effect += spells[s].damage_effect;
    player_armor += spells[s].armor_effect;
  }

  // Apply damage from damage effect spells
  boss.hitpoints -= damage_effect;
  if (boss.hitpoints <= 0) {
    return player.mana_spent; // player wins
  }

  if (turn == PLAYER) {

    // Apply hitpoints drain (part 2)
    player.hitpoints -= hitpoints_drain;
    if (player.hitpoints <= 0) {
      return INT_MAX;
    }

    // Cast all 5 spells and recurse
    for (unsigned s = 0; s < 5; s++) {
      if (timers[s] > 0 || spells[s].mana_cost > player.mana) {
        continue;
      }

      struct player_stats new_player = player;
      new_player.mana -= spells[s].mana_cost;
      new_player.mana_spent += spells[s].mana_cost;

      // Prune if we're already above best game
      if (new_player.mana_spent > least_mana_spent) {
        return least_mana_spent;
      }

      unsigned char new_timers[5];
      memcpy(new_timers, timers, 5 * sizeof(unsigned char));
      new_timers[s] = spells[s].time_effect;

      struct boss_stats new_boss = boss;
      new_boss.hitpoints -= spells[s].damage;
      new_player.hitpoints += spells[s].heal;
      unsigned int spent = fight(new_boss, new_player, new_timers, !turn,
                                 least_mana_spent, hitpoints_drain);
      if (spent < least_mana_spent) {
        least_mana_spent = spent;
      }
    }

    return least_mana_spent;
  }

  int dmg = (boss.damage - player_armor) > 1 ? boss.damage - player_armor : 1;
  player.hitpoints -= dmg;

  return fight(boss, player, timers, !turn, least_mana_spent, hitpoints_drain);
}

int main(void) {
  clock_t start_t = clock_time();
  char input[1024];
  read_input_file(input, 1024, "input.txt");

  struct boss_stats boss = parse(input);
  struct player_stats player = {50, 500, 0};
  unsigned char spell_timers[] = {0, 0, 0, 0, 0};

  unsigned int pt1 = fight(boss, player, spell_timers, PLAYER, INT_MAX, 0);
  unsigned int pt2 = fight(boss, player, spell_timers, PLAYER, INT_MAX, 1);

  printf("--- Day 22: Wizard Simulator 20XX ---\n");
  printf("Part 1: %d\n", pt1);
  printf("Part 2: %d\n", pt2);
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}

// 113 too high
