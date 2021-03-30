#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>

#define HASH_MAX 1298779
#define NDEBUG 1

typedef struct player player_t;
struct player {
    int8_t deck[50];
    int8_t ncards;
};

typedef struct game game_t;
struct game {
    player_t p1;
    player_t p2;
    int64_t index;
    int64_t round;
    bool ended;
    int8_t *previous_decks;
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
    game->previous_decks = calloc(100000 * 100, sizeof(int8_t));

    //memset(game->hashes, 0, HASH_MAX * sizeof(int8_t));
    return game;
}

game_t *
copy_game(game_t *g1, int8_t card_p1, int8_t card_p2) {
    game_t *g2 = new_game();
    g2->index = g1->index + 1;
    g2->p1.ncards = card_p1;
    for (int8_t i=0; i < card_p1; i++) {
        g2->p1.deck[i] = g1->p1.deck[i];
    }
    g2->p2.ncards = card_p2;
    for (int8_t i=0; i < card_p2; i++) {
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
        int8_t n = 0;
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
        int8_t n = 0;
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

int8_t 
shift_card_from_deck(player_t *p) {
    int8_t card = p->deck[0];
    for (int8_t i=0; i < p->ncards - 1; i++) {
        p->deck[i] = p->deck[i+1];
    }
    p->ncards--;
    return card;
}

void
add_card_to_deck(player_t *p, int8_t card) {
    p->deck[p->ncards++] = card;
}

void
print_player_deck(player_t *p) {
    for (int8_t i=0; i < p->ncards; i++) {
        printf("%s%d", i > 0 ? ", " : "", p->deck[i]);
    }
}

// int64_t 
// hash_decks(player_t *p1, player_t *p2) {
//     unsigned long hash = 5381;
//     int c;
//     char str[BUFSIZ] = {0};
//     for (size_t i = 0; i < p1->ncards; i++) {
//         sprintf(str, "%s%d", str, p1->deck[i]);
//     }

//     char *s = str;

//     while (c = *s++)
//         hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

//     return hash % HASH_MAX;
// }

bool 
find_deck_in_game(game_t *g) {
    int8_t j;
   
    for (int64_t i=0; i < g->round; i++) {
        for (j=0; j < g->p1.ncards; j++) {
            if (g->previous_decks[(i * 100) + j] != g->p1.deck[j]) {
                break;   
            }
        }
        
        if (j == g->p1.ncards) {
            // deck for player 1 matched
            for (j=0; j < g->p2.ncards; j++) {
                if (g->previous_decks[(i * 100) + 50 + j] != g->p2.deck[j]) {
                    break;   
                }
            }

            if (j == g->p2.ncards) {
                return true;
            }
        }
    }

    return false;
}

player_t * 
play_round(game_t *g) {
    player_t *p1 = &(g->p1);
    player_t *p2 = &(g->p2);
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
    // if (find_deck_in_game(g)) {
        #ifndef NDEBUG
        printf("Player 1 wins because we've been here before!\n");
        #endif
        g->ended = true;
        return p1;
    }
    // for (size_t i = 0; i < p1->ncards; i++) {
    //     g->previous_decks[(g->round-1) * 100 + i] = p1->deck[i];
    // }
    // for (size_t i = 0; i < p2->ncards; i++) {
    //     g->previous_decks[(g->round-1) * 100 + 50 + i] = p2->deck[i];
    // }

    // g->hashes[hash] = 1;

    // draw top card (index 0) from both decks
    // move them both to the player that played the highest card
    // with the highest card on top
    int32_t card_p1 = shift_card_from_deck(p1);
    int32_t card_p2 = shift_card_from_deck(p2);
    #ifndef NDEBUG
    printf("Player 1 plays: %d\n", card_p1);
    printf("Player 2 plays: %d\n", card_p2);
    #endif

    if (g->index > 1 && card_p1 > (p1->ncards + p2->ncards)) {
        g->ended = true;
        return p1;
    }

    // TODO: Check ncards of both players
    // If it is equal or higher than value of card they drew
    // Start a sub-game
    bool played_subgame = false;
    if (p1->ncards >= card_p1 && p2->ncards >= card_p2) {
        #ifndef NDEBUG
        printf("Playing a sub-game to determine the winner.\n");
        #endif
        game_t *sub_game = copy_game(g, card_p1, card_p2);
        winner = play_game(sub_game);
        free(sub_game->previous_decks);
        free(sub_game);
        played_subgame = true;
    }

    //printf("Drawed %ld and %ld\n", card_p1, card_p2);
    if ((played_subgame == false && card_p1 > card_p2) || (played_subgame == true && winner == p1)) {
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
    game_t *game = parse_input("test_input_3.txt");
    player_t *winner = play_game(game);
    int64_t score = 0;

    printf("Winner's deck: ");
    print_player_deck(winner);
    printf("\n");

    for (int8_t i=0; i < winner->ncards; i++) {
        score = score + ( winner->deck[i] * (winner->ncards - i));
    }

    printf("Score: %ld (%s)", score, score == 34031 ? "CORRECT" : "WRONG");

    // day_input_3.txt = 34031
    // 45, 36, 38, 32, 37, 2, 41, 16, 39, 5, 23, 12, 19, 11, 46, 43, 21, 13, 33, 14, 29, 10, 49, 40, 48, 30, 35, 27, 31, 4, 34, 25, 24, 1, 28, 20, 26, 8, 22, 3, 9, 6, 50, 15, 47, 7, 44, 18, 42, 17,

    // assert(score != 32977);
    free(game->previous_decks);
    free(game);
}   

//