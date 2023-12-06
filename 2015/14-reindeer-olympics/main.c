#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

static inline
char *parse_int(int *dst, char *s) {
    int n = 0;
    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }
    *dst = n;
    return s;
}

static inline
char *parse_ident(char *dst, char *s) {
    while (*s != ' ' && *s != '\0') {
        *dst++ = *s++;
    }
    *dst = '\0';
    return s;
}

typedef struct reindeer {
    char name[10];
    int speed;
    int fly_time;
    int rest_time;
    int distance;
    int flying;
    int toggle_at;
    int points;
} reindeer_t;

int pt1(reindeer_t players[], int nplayers) {
    int a = 0;

    for (int i = 0; i < nplayers; i++) {
        reindeer_t p = players[i];
        int cycle_time = p.fly_time + p.rest_time;
        double cycles = 2503 / (double) cycle_time;
        int distance_traveled = (int) cycles * p.fly_time * p.speed;
        // get max of leftover flying time
        double leftover_flying_time = (double) (int) cycles * cycle_time;
        if (leftover_flying_time > p.fly_time) {
            leftover_flying_time = p.fly_time;
        }
        distance_traveled += leftover_flying_time * p.speed;
        if (distance_traveled > a) {
            a = distance_traveled;
        }
    }

    return a;
}

int max_points(reindeer_t players[], int nplayers) {
    int max_points = 0;

    for (int n = 0; n < nplayers; n++) {
        if (players[n].points > max_points) {
            max_points = players[n].points;
        }
    }

    return max_points;
}

reindeer_t get_leader(reindeer_t players[], int nplayers) {
    reindeer_t leader = players[0];

    for (int n = 1; n < nplayers; n++) {
        if (players[n].distance > leader.distance) {
            leader = players[n];
        }
    }

    return leader;
}

int pt2(reindeer_t players[], int nplayers) {
    for (int n = 0; n < nplayers; n++) {
        players[n].flying = 1;
        players[n].toggle_at = players[n].fly_time;
    }

    for (int i = 0; i < 2503; i++) {
        for (int n = 0; n < nplayers; n++) {
            if (players[n].toggle_at == i) {
                players[n].flying = players[n].flying == 1 ? 0 : 1;
                players[n].toggle_at += players[n].flying ? players[n].fly_time : players[n].rest_time;
            }

            if (players[n].flying) {
                players[n].distance += players[n].speed;
            }
        }

        reindeer_t leader = get_leader(players, nplayers);
        for (int n = 0; n < nplayers; n++) {
            if (players[n].distance >= leader.distance) {
                players[n].points++;
            }
        }

    }

    return max_points(players, nplayers);
}

void parse(reindeer_t *players, int *nplayers) {
    FILE *fp = fopen("input.txt", "r");
    char *input = (char *) malloc(1024 * 32);
    size_t nread = fread(input, 1, 1024*32, fp);
    fclose(fp);
    input[nread] = '\0';

    int n = 0;
    char *s = input;
    while (*s != '\0') {
        s = parse_ident(players[n].name, s);
        while (*s < '0' || *s > '9') s++;
        s = parse_int(&players[n].speed, s);
        while (*s < '0' || *s > '9') s++;
        s = parse_int(&players[n].fly_time, s);
        while (*s < '0' || *s > '9') s++;
        s = parse_int(&players[n].rest_time, s);
        while (*s != '\n') s++;

        players[n].points = 0;
        players[n].toggle_at = 0;
        players[n].flying = 0;
        players[n].distance = 0;
        s++;
        n++;
    }

    *nplayers = n;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    reindeer_t players[10];
    int nplayers = 0;
    parse(players, &nplayers);

    printf("--- Day 14: Reindeer Olympics ---\n");

    int a1 = pt1(players, nplayers);
    printf("Part 1: %d\n", a1);
    assert(a1 == 2660);

    int a2 = pt2(players, nplayers);
    printf("Part 2: %d\n", a2);
    assert(a2 == 1256);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}