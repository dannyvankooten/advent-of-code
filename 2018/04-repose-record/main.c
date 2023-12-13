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

struct guard {
    int id;
    int minutes[60];
    int total_sleep_time;
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

    qsort(log_entries, nentries, sizeof(struct log_entry), log_entry_compare);
    return nentries;
}

struct guard* guard_by_id(struct guard *guards, int nguards, int id) {
    for (int i = 0; i < nguards; i++) {
        if (guards[i].id == id) {
            return &guards[i];
        }
    }

    return NULL;
}

int log_entries_into_guards(struct guard *guards, struct log_entry *entries, int nentries) {
    int nguards = 0;
    struct guard *guard = NULL;

    for (int i=0; i < nentries; i++) {
        struct log_entry e = entries[i];

        if (e.guard_id > 0) {
            guard = guard_by_id(guards, nguards, e.guard_id);
            if (guard == NULL) {
                guard = &guards[nguards++];
                guard->id = e.guard_id;
                guard->total_sleep_time = 0;
                memset(guard->minutes, 0, 60 * sizeof(*guard->minutes));
            }
        } else if (e.awake > 0) {
            if (i == 0 || guard == NULL) {
              fprintf(stderr, "logic error");
              exit(EXIT_FAILURE);
            }
            struct log_entry  prev = entries[i-1];
            guard->total_sleep_time += (e.minute - prev.minute);
            for (int m = prev.minute; m < e.minute; m++) {
                guard->minutes[m]++;
            }
        }
    }
    return nguards;
}

int solve_pt1(struct guard *guards, int nguards) {
   struct guard *guard = &guards[0];

   for (int i = 0; i < nguards; i++) {
        if (guards[i].total_sleep_time > guard->total_sleep_time) {
            guard = &guards[i];
        }
    }

    int sleepy_minute = 0;
    for (int m = 0; m < 60; m++) {
        if (guard->minutes[m] > guard->minutes[sleepy_minute]) {
            sleepy_minute = m;
       }
    }

    int answer_pt1 = guard->id * sleepy_minute;
    return answer_pt1;
}

int solve_pt2(struct guard *guards, int nguards) {
   struct guard *guard = &guards[0];
   int max = 0;

   for (int i = 0; i < nguards; i++) {
       for (int m = 0; m < 60; m++) {
           if (guards[i].minutes[m] > guard->minutes[max]) {
               guard = &guards[i];
               max = m;
           }
       }
    }

    return guard->id * max;
}

int main() {
    clock_t t = timer_start();
    char input[1024 * 64] = "";
    read_input_file(input, 1024 * 64, "input.txt");

    struct log_entry *log_entries = malloc_or_die(2048 * sizeof (struct log_entry));
    int nentries = parse(log_entries, input);

    struct guard *guards = malloc_or_die(512 * sizeof(struct guard));
    int nguards = log_entries_into_guards(guards, log_entries, nentries);
    int pt1 = solve_pt1(guards, nguards);
    int pt2 = solve_pt2(guards, nguards);

    printf("--- %s ---\n", PUZZLE_NAME);
    printf("Part 1: %d\n", pt1);
    printf("Part 2: %d\n", pt2);
    printf("Time: %.2fms\n", timer_stop(t));
    free(guards);
    free(log_entries);
    return EXIT_SUCCESS;
}
