#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../adventofcode.h"

#define PUZZLE_NAME "Day 4: Repose Record"

struct log_entry {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int guard_id;
    int awake;
};

static int log_entry_compare(const void *p1, const void *p2) {
    struct log_entry *a = (struct log_entry *) p1;
    struct log_entry *b = (struct log_entry *) p2;

    if (a->year < b->year) {
        return -1;
    }

    if (a->year == b->year) {
        if (a->month < b->month) {
            return -1;
        }

        if (a->month == b->month) {
            if (a->day < b->day) {
                return -1;
            }

            if (a->day == b->day) {
                if (a->hour < b->hour) {
                    return -1;
                }

                if (a->hour == b->hour) {
                    return a->minute - b->minute;
                }
            }
        }
    }

    return 1;
}

int parse(struct log_entry *log_entries, char *s) {
    int nentries = 0;
    while (*s != 0x0) {
        struct log_entry *e = &log_entries[nentries++];
        e->guard_id = 0;
        e->awake = 0;
        sscanf(s, "[%d-%02d-%02d %02d:%02d]", &e->year, &e->month, &e->day, &e->hour, &e->minute);
        s += 19; // skip timestamp

        switch (*s) {
            case 'G':
                sscanf(s, "Guard #%d", &e->guard_id);
                break;
            case 'f':
                e->awake = 0;
                break;

            case 'w':
                e->awake = 1;
                break;
        }

        while (*s != '\n' && *s != 0x0) s++;
        if (*s == '\n') s++;
    }
    return nentries;
}

int solve(struct log_entry *entries, int nentries) {
    int sleepcount[100];
    memset(sleepcount, 0, 100 * sizeof(int));

    int guard;
    for (int i=0; i < nentries; i++) {
        struct log_entry e = entries[i];

        if (e.guard_id > 0) {
            printf("%d starts shift\n", e.guard_id);
            guard = e.guard_id;
        } else if (e.awake > 0) {
            printf("Guard %d wakes up, adding %d minutes\n", guard, e.minute - entries[i-1].minute);
            sleepcount[guard] += e.minute - entries[i-1].minute;
        }
    }

    int max_idx = -1;
    int max_value = 0;
    for (int i = 0; i < 100; i++) {
        if (sleepcount[i] > max_value) {
            max_value = sleepcount[i];
            max_idx = i;
        }
    }

    printf("Guard %d sleeps the most: %d minutes\n", max_idx, max_value);
}

int main() {
    clock_t t = timer_start();
    char input[1024 * 64];
    read_input_file(input, "input_test.txt");

    struct log_entry *log_entries = malloc(2048 * sizeof (struct log_entry));
    int nentries = parse(log_entries, input);

    printf("Scanned %d entries\n", nentries);
    qsort(log_entries, nentries, sizeof(struct log_entry), log_entry_compare);
    for (int i =0; i < nentries; i++) {
        struct log_entry *e = &log_entries[i];
        printf("[%04d-%02d-%02d %02d:%02d] guard:%d awake:%d\n", e->year, e->month, e->day, e->hour, e->minute, e->guard_id, e->awake);
    }

    int a1 = solve(log_entries, nentries);

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", a1);
    printf("Part 2: %d\n", 0);
    printf("Time: %.2fms\n", timer_stop(t));
    free(log_entries);
    return EXIT_SUCCESS;
}
