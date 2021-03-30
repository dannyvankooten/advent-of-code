#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>

#define HASH_MAX 12855111
#define NDEBUG 1

typedef struct player player_t;
struct player {
    int16_t deck[50];
    size_t ncards;
};

typedef struct game game_t;
struct game {
    player_t p1;
    player_t p2;
    int64_t index;
    int64_t round;
    bool ended;
    // int8_t hashes[HASH_MAX];
};

player_t *play_game(game_t * g);

game_t *
new_game() {
    game_t *game = malloc(sizeof(game_t));
    game->p1.ncards = 0;
    game->p2.ncards = 0;
    game->ended = false;
    game->index = 1;
    game->round = 0;
    // memset(game->hashes, 0, HASH_MAX * sizeof(int8_t));
    return game;
}

game_t *
copy_game(game_t *g1, int16_t card_p1, int16_t card_p2) {
    game_t *g2 = new_game();
    g2->index = g1->index + 1;
    g2->p1.ncards = card_p1;
    for (int16_t i=0; i < card_p1; i++) {
        g2->p1.deck[i] = g1->p1.deck[i];
    }
    g2->p2.ncards = card_p2;
    for (int16_t i=0; i < card_p2; i++) {
        g2->p2.deck[i] = g1->p2.deck[i];
    }
    return g2;
}

game_t *
parse_input(char *file) {
    game_t *game = new_game();
    FILE *f = fopen(file, "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};

    // skip line: "Player 1:"
    char *s = fgets(linebuf, BUFSIZ, f); 
    assert(s != NULL);
    
    while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
        int16_t n = 0;
        s = linebuf;
        while (*s >= '0' && *s <= '9') {
            n = (n * 10) + (*s - '0');
            s++;
        }
        game->p1.deck[game->p1.ncards++] = n;
    }   

    // skip line: "Player 2:"
    s = fgets(linebuf, BUFSIZ, f);
    assert(s != NULL);

    while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
        int16_t n = 0;
        s = linebuf;
        while (*s >= '0' && *s <= '9') {
            n = (n * 10) + (*s - '0');
            s++;
        }
        game->p2.deck[game->p2.ncards++] = n;
    }   
    
    fclose(f);
    return game;
}

int16_t 
shift_card_from_deck(player_t *p) {
    int16_t card = p->deck[0];
    for (size_t i=1; i < p->ncards; i++) {
        p->deck[i-1] = p->deck[i];
    }
    p->ncards--;
    return card;
}

void
add_card_to_deck(player_t *p, int16_t card) {
    p->deck[p->ncards++] = card;
}

void
print_player_deck(player_t *p) {
    for (size_t i=0; i < p->ncards; i++) {
        printf("%s%d", i > 0 ? ", " : "", p->deck[i]);
    }
}

int64_t 
hash_decks(player_t *p1, player_t *p2) {
    u_int64_t result = 0;
    
    for (size_t i=0; i < p1->ncards; i++) {
        result = (result * 10) + p1->deck[i];
    }
    for (size_t i=0; i < p2->ncards; i++) {
        result = (result * 10) + p2->deck[i];
    }

    return result % HASH_MAX;
}

player_t * 
play_round(game_t *g) {
    player_t *p1 = &g->p1;
    player_t *p2 = &g->p2;
    player_t *winner = NULL;
    player_t *loser = NULL;
    g->round++;

    // int64_t hash = hash_decks(p1, p2);

    #ifndef NDEBUG
    printf("-- Round %" PRId64 " (Game %" PRId64 ") --\n", g->round, g->index);
    // printf("Hash: %" PRId64 "\n", hash);
    printf("Player 1's deck: ");
    print_player_deck(&g->p1);
    printf("\nPlayer 2's deck: ");
    print_player_deck(&g->p2);
    printf("\n");
    #endif

    // If we've seen this exact same configuration before this game
    // Player 1 wins
    if (g->round > 1500) {
    // if (g->hashes[hash] == 1) {
        #ifndef NDEBUG
        printf("Player 1 wins because we've been here before!\n");
        #endif
        g->ended = true;
        return p1;
    } 
    // g->hashes[hash] = 1;

    // draw top card (index 0) from both decks
    // move them both to the player that played the highest card
    // with the highest card on top
    int16_t card_p1 = shift_card_from_deck(p1);
    int16_t card_p2 = shift_card_from_deck(p2);
    #ifndef NDEBUG
    printf("Player 1 plays: %d\n", card_p1);
    printf("Player 2 plays: %d\n", card_p2);
    #endif

    // TODO: Check ncards of both players
    // If it is equal or higher than value of card they drew
    // Start a sub-game
    if (p1->ncards >= (size_t) card_p1 && p2->ncards >= (size_t) card_p2) {
        #ifndef NDEBUG
        printf("Playing a sub-game to determine the winner.\n");
        #endif
        game_t *sub_game = copy_game(g, card_p1, card_p2);
        winner = play_game(sub_game);
        free(sub_game);
    }

    //printf("Drawed %ld and %ld\n", card_p1, card_p2);
    if ((winner == NULL && card_p1 > card_p2) || winner == p1) {
        add_card_to_deck(p1, card_p1);
        add_card_to_deck(p1, card_p2);
        winner = p1;
        loser = p2;
    } else {
        add_card_to_deck(p2, card_p2);
        add_card_to_deck(p2, card_p1);
        winner = p2;
        loser = p1;
    }

    #ifndef NDEBUG
    printf("Player %d wins round %" PRId64 " of game %" PRId64 "!\n", winner == p1 ? 1 : 2, g->round, g->index);
    #endif

    if (loser->ncards == 0) {
        g->ended = true;

        #ifndef NDEBUG
        printf("The winner of game %" PRId64 " is player %d!\n", g->index, winner == p1 ? 1 : 2);
        #endif
    }

    return winner;
}

player_t *
play_game(game_t *game) {
    player_t *winner = NULL;
    while (!game->ended) {
        winner = play_round(game);
    }
    assert(winner != NULL);
    return winner;
}

int 
main() {
    game_t *game = parse_input("input.txt");
    player_t *winner = play_game(game);
    int64_t score = 0;
    for (size_t i=0; i < winner->ncards; i++) {
        score = score + ( winner->deck[i] * (winner->ncards - i));
    }

    printf("Part 1: %ld", score);
    // assert(score != 32977);
    free(game);
}   

//