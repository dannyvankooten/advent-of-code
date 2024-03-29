#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inputs/22.h"
#define NDEBUG 1

typedef struct player player_t;
struct player {
  uint16_t offset;
  uint8_t deck[50];
  uint8_t ncards;
};

typedef struct game game_t;
struct game {
  player_t p1;
  player_t p2;
  uint16_t index;
  uint16_t round;
  player_t* winner;
};

static player_t* play_game(game_t* g);

game_t* games_memory;
size_t games_memory_ptr = 0;

static game_t*
new_game(void) {
  game_t* game = &games_memory[games_memory_ptr++];
  game->p1.ncards = 0;
  game->p1.offset = 0;
  game->p2.ncards = 0;
  game->p2.offset = 0;
  game->winner = NULL;
  game->index = 1;
  game->round = 0;
  return game;
}

static game_t*
copy_game(game_t* g1, const int8_t card_p1, const int8_t card_p2) {
  game_t* g2 = new_game();
  g2->index = g1->index + 1;
  g2->p1.ncards = card_p1;
  int8_t largest_card_p1 = 0;
  int8_t largest_card_p2 = 0;
  for (int8_t i = 0; i < card_p1; i++) {
    g2->p1.deck[i] = g1->p1.deck[(g1->p1.offset + i) % 50];
    if (g2->p1.deck[i] > largest_card_p1) {
      largest_card_p1 = g2->p1.deck[i];
    }
  }
  g2->p2.ncards = card_p2;
  for (int8_t i = 0; i < card_p2; i++) {
    g2->p2.deck[i] = g1->p2.deck[(g1->p2.offset + i) % 50];
    if (g2->p2.deck[i] > largest_card_p2) {
      largest_card_p2 = g2->p2.deck[i];
    }
  }

  // if sub-game starts with player 1 holding the highest card
  // immediately declare him as winner
  if (largest_card_p1 > largest_card_p2) {
    g2->winner = &g2->p1;

  }
  return g2;
}

static game_t*
parse_input(void) {
  game_t* game = new_game();
  const unsigned char *s = input;

  // skip line: "Player 1:"
  while (*s != '\n') s++;
  s++; // skip '\n'

  while (*s != '\n') {
    int8_t n = 0;
    while (*s >= '0' && *s <= '9') {
      n = (n * 10) + (*s - '0');
      s++;
    }
    game->p1.deck[game->p1.ncards++] = n;
    s++; // skip '\n'
  }

  s++; // skip empty newline

  // skip line: "Player 2:"
  while (*s != '\n') s++;
  s++;

  while (*s != '\0') {
    int8_t n = 0;
    while (*s >= '0' && *s <= '9') {
      n = (n * 10) + (*s - '0');
      s++;
    }

    game->p2.deck[game->p2.ncards++] = n;

    while (*s == '\n') s++; // skip trailing '\n'
  }

  return game;
}

static int8_t
shift_card_from_deck(player_t* restrict p) {
  int8_t card = p->deck[p->offset % 50];
  p->offset++;
  p->ncards--;
  return card;
}

static void
add_card_to_deck(player_t* restrict p, const int8_t card) {
  p->deck[(p->offset + p->ncards) % 50] = card;
  p->ncards++;
}

__attribute((unused))
static void
print_player_deck(const player_t* restrict p) {
  for (int8_t i = 0; i < p->ncards; i++) {
    printf("%s%d", i > 0 ? ", " : "", p->deck[(p->offset + i) % 50]);
  }
}

static player_t*
play_round(game_t* restrict g) {
  player_t* p1 = &(g->p1);
  player_t* p2 = &(g->p2);
  player_t* winner = NULL;
  player_t* loser = NULL;
  g->round++;

#ifndef NDEBUG
  printf("-- Round %" PRId64 " (Game %" PRId64 ") --\n", g->round, g->index);
  printf("Player 1's deck: ");
  print_player_deck(&g->p1);
  printf("\nPlayer 2's deck: ");
  print_player_deck(&g->p2);
  printf("\n");
#endif

  // If we've seen this exact same configuration before this game
  // Player 1 wins
  if (g->round > 1000) {
    // if (find_deck_in_game(g)) {
#ifndef NDEBUG
    printf("Player 1 wins because we've been here before!\n");
#endif
    g->winner = p1;
    return p1;
  }

  // draw top card (index 0) from both decks
  // move them both to the player that played the highest card
  // with the highest card on top
  int32_t card_p1 = shift_card_from_deck(p1);
  int32_t card_p2 = shift_card_from_deck(p2);
#ifndef NDEBUG
  printf("Player 1 plays: %d\n", card_p1);
  printf("Player 2 plays: %d\n", card_p2);
#endif

  if (card_p1 <= p1->ncards && card_p2 <= p2->ncards) {
#ifndef NDEBUG
    printf("Playing a sub-game to determine the winner.\n");
#endif
    game_t* sub_game = copy_game(g, card_p1, card_p2);
    winner = play_game(sub_game);
    winner = (winner == &sub_game->p1) ? p1 : p2;
    games_memory_ptr--;
  }

  // printf("Drawed %ld and %ld\n", card_p1, card_p2);
  if ((winner == NULL && card_p1 > card_p2) || (winner == p1)) {
    // P1 won
    add_card_to_deck(p1, card_p1);
    add_card_to_deck(p1, card_p2);
    winner = p1;
    loser = p2;
  } else {
    // P2 won
    add_card_to_deck(p2, card_p2);
    add_card_to_deck(p2, card_p1);
    winner = p2;
    loser = p1;
  }

#ifndef NDEBUG
  printf("Player %d wins round %" PRId64 " of game %" PRId64 "!\n",
         winner == p1 ? 1 : 2, g->round, g->index);
#endif

  if (loser->ncards == 0) {
    g->winner = winner;

#ifndef NDEBUG
    printf("The winner of game %" PRId64
           " is player %d!\nGame lasted %ld rounds.\n",
           g->index, winner == p1 ? 1 : 2, g->round);
#endif
  }

  return winner;
}

static player_t*
play_game(game_t* restrict game) {
  while (game->winner == NULL) {
    play_round(game);
  }
  assert(game->winner != NULL);
  return game->winner;
}

int day22(void) {
  // pre-allocate memory for main game and enough sub-games
  games_memory = malloc(50 * sizeof(game_t));
  if (!games_memory) {
    err(EXIT_FAILURE, "error allocating memory for games");
  }

  game_t* game = parse_input();
  player_t* winner = play_game(game);
  int64_t score = 0;

  for (int8_t i = 0; i < winner->ncards; i++) {
    score = score +
            (winner->deck[(winner->offset + i) % 50] * (winner->ncards - i));
  }
  printf("%ld\n", score);
  assert(score == 33469);
  games_memory_ptr = 0;
  free(games_memory);
  return 0;
}
