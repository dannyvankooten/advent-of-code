#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "../adventofcode.h"

#define PUZZLE_NAME "Day 7: The Sum Of Its Parts"

struct step {
  char name;
  char done;
  char busy;
  int nprereqs;
  struct step *prereqs[16];
};

// step_byname finds the step with the given name
static struct step *step_byname(struct step *steps, int nsteps, char name) {
  for (int i = 0; i < nsteps; i++) {
    if (steps[i].name == name) {
      return &steps[i];
    }
  }

  return NULL;
}

// step_next finds the first available step that's not already done in an alphabetically sorted list of steps
static struct step *step_next(struct step **steps, int nsteps) {
  for (int i = 0; i < nsteps; i++) {
    struct step *a = steps[i];
    if (a->done || a->busy) continue;

    int ready = 1;
    for (int r = 0; r < a->nprereqs; r++) {
      if (a->prereqs[r]->done == 0) {
        ready = 0;
        break;
      }
    }

    if (ready == 1) {
      return a;
    }
  }

  return NULL;
}

int step_compare(const void *p1, const void *p2) {
  struct step *a = *(struct step **)p1;
  struct step *b = *(struct step **)p2;
  return a->name - b->name;
}

void pt1(char *buf, struct step **steps, int nsteps) {
  char *o = buf;
  while (1) {
    struct step *cur = step_next(steps, nsteps);
    if (cur == NULL) break;
    *o++ = cur->name;
    cur->done = 1;
  }
  *o = 0x0;

  // clean-up: mark all tasks as unfinished again
  for (int i = 0; i < nsteps; i++) steps[i]->done = 0;
}

int pt2(struct step **steps, int nsteps) {
  int nworkers = 5;
  int delay_s = 60;
  int worker_finish_times[nworkers];
  struct step *worker_tasks[nworkers];
  memset(worker_finish_times, 0, sizeof(*worker_finish_times) * nworkers);
  memset(worker_tasks, 0, sizeof(struct step *) * nworkers);
  int ndone = 0;

  // loop over seconds
  for (int s = 0;; s++) {

    // first loop, mark steps as done
    for (int w = 0; w < nworkers; w++) {
      if (worker_finish_times[w] != s) {
        continue;
      }

      struct step *step = worker_tasks[w];
      if (step != NULL) {
        step->done = 1;

        // exit condition: finish when we marked all tasks as done
        if (++ndone == nsteps) {
          return s;
        }
      }

      worker_tasks[w] = NULL;
      worker_finish_times[w] = s + 1;
    }

    // second loop: assign new tasks
    for (int w = 0; w < nworkers; w++) {
      if (worker_tasks[w] != NULL) {
        continue;
      }

      // assign new task
      struct step *step = step_next(steps, nsteps);
      if (step != NULL) {
        step->busy = 1;
        worker_tasks[w] = step;
        worker_finish_times[w] = s + delay_s + (step->name - 'A' + 1);
      }
    }
  }

  return -1;
}

int main() {
  clock_t t = timer_start();
  char input[1024 * 64] = "";
  read_input_file(input, 1024 * 64, "input.txt");

  struct step steps[64];
  int nsteps = 0;

  char *s = input;
  char name;
  char name_prereq;
  while (*s != 0x0) {
    s = skip("Step ", s);
    name_prereq = *s++;
    s = skip(" must be finished before step ", s);
    name = *s++;
    while (*s != '\n' && *s != 0x0) s++;
    if (*s == '\n') s++;

    struct step *step = step_byname(steps, nsteps, name);
    if (step == NULL) {
      step = &steps[nsteps++];
      step->name = name;
      step->nprereqs = 0;
      step->done = 0;
      step->busy = 0;
    }

    struct step *prereq = step_byname(steps, nsteps, name_prereq);
    if (prereq == NULL) {
      prereq = &steps[nsteps++];
      prereq->name = name_prereq;
      prereq->nprereqs = 0;
      prereq->done = 0;
      prereq->busy = 0;
    }

    step->prereqs[step->nprereqs++] = prereq;
  }

  struct step *steps_sorted[64];
  for (int i = 0; i < nsteps; i++) {
    steps_sorted[i] = &steps[i];
  }
  qsort(steps_sorted, nsteps, sizeof (struct step *), step_compare);


  // solve for pt 1
  char a1[64];
  pt1(a1, steps_sorted, nsteps);

  int a2 = pt2(steps_sorted, nsteps);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %s\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", timer_stop(t));
  return EXIT_SUCCESS;
}
