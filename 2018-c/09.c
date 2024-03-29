#include "adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 9: Marble Mania"

// Puzzle input
#define NPLAYERS 458
#define NMARBLES 72019

struct node {
  unsigned long value;
  struct node *next;
  struct node *prev;
};

static unsigned long max(const unsigned long arr[static NPLAYERS]) {
  unsigned int mi = 1;
  for (unsigned int i = 2; i < NPLAYERS; i++) {
    if (arr[i] > arr[mi]) {
      mi = i;
    }
  }

  return arr[mi];
}

static unsigned long play(const unsigned long nmarbles) {
  struct node *marbles = malloc_or_die(sizeof(struct node) * (nmarbles + 1));
  for (unsigned long m = 0; m <= nmarbles; m++) {
    marbles[m].value = m;
    marbles[m].prev = NULL;
    marbles[m].next = NULL;
  }

  unsigned long scores[NPLAYERS] = {0};
  struct node *cur = &marbles[0];
  cur->next = cur;
  cur->prev = cur;
  unsigned int player = 0;
  for (unsigned long m = 1; m <= nmarbles; m++, player++, player %= NPLAYERS) {

    if (m % 23 == 0) {
      scores[player] += m;
      struct node *item = cur;
      for (unsigned int i = 0; i < 7; i++) {
        item = item->prev;
      }
      scores[player] += item->value;

      item->prev->next = item->next;
      item->next->prev = item->prev;
      cur = item->next;
      continue;
    }

    struct node *new = &marbles[m];
    new->prev = cur->next;
    new->next = cur->next->next;
    cur->next->next->prev = new;
    cur->next->next = new;
    cur = new;
  }

  free(marbles);
  return max( scores);
}

int main(void) {
  clock_t t = clock_time();

  unsigned long pt1 = play(NMARBLES);
  unsigned long pt2 = play(NMARBLES * 100);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %ld\n", pt1);
  printf("Part 2: %ld\n", pt2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
