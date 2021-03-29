#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <stdbool.h>

typedef struct player player_t;
struct player {
    int8_t deck[50];
    size_t ncards;
};

typedef struct game game_t;
struct game {
    player_t p1;
    player_t p2;
    bool ended;
};

game_t *
parse_input(char *file) {
    game_t *game = malloc(sizeof(game_t));
    game->p1.ncards = 0;
    game->p2.ncards = 0;
    game->ended = false;
    
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
    for (size_t i=1; i < p->ncards; i++) {
        p->deck[i-1] = p->deck[i];
    }
    p->ncards--;
    return card;
}

void
add_card_to_deck(player_t *p, int8_t card) {
    p->deck[p->ncards++] = card;
}

player_t * 
play_round(game_t *g) {
    player_t *p1 = &g->p1;
    player_t *p2 = &g->p2;
    player_t *winner = NULL;
    player_t *loser = NULL;

    // draw top card (index 0) from both decks
    // move them both to the player that played the highest card
    // with the highest card on top
    int8_t card_p1 = shift_card_from_deck(p1);
    int8_t card_p2 = shift_card_from_deck(p2);

    //printf("Drawed %ld and %ld\n", card_p1, card_p2);
    if (card_p1 > card_p2) {
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

    if (loser->ncards == 0) {
        g->ended = true;
    }

    return winner;
}

int main() {
    game_t *game = parse_input("input.txt");
    player_t *winner = NULL;

    while (!game->ended) {
        winner = play_round(game);
    }
    assert(winner != NULL);

    int64_t score = 0;
    for (size_t i=0; i < winner->ncards; i++) {
        score += winner->deck[i] * (winner->ncards - i);
    }

    printf("Score: %ld", score);
    free(game);
}   
