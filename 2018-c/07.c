#include "adventofcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PUZZLE_NAME "Day 7: The Sum Of Its Parts"

struct step {
  char name;
  char done;
  char busy;
  size_t nprereqs;
  struct step *prereqs[16];
};

// step_byname finds the step with the given name
static struct step *step_byname(struct step *steps, size_t nsteps, char name) {
  for (size_t i = 0; i < nsteps; i++) {
    if (steps[i].name == name) {
      return &steps[i];
    }
  }

  return NULL;
}

// step_next finds the first available step that's not already done in an
// alphabetically sorted list of steps
static struct step *step_next(struct step **steps, size_t nsteps) {
  for (size_t i = 0; i < nsteps; i++) {
    struct step *a = steps[i];
    if (a->done || a->busy)
      continue;

    int ready = 1;
    for (size_t r = 0; r < a->nprereqs; r++) {
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

static int step_compare(const void *p1, const void *p2) {
  const struct step *a = *(struct step **)p1;
  const struct step *b = *(struct step **)p2;
  return (a->name > b->name) - (a->name < b->name);
}

static void pt1(char *buf, struct step **steps, const size_t nsteps) {
  char *o = buf;
  while (1) {
    struct step *cur = step_next(steps, nsteps);
    if (cur == NULL)
      break;
    *o++ = cur->name;
    cur->done = 1;
  }
  *o = 0x0;

  // clean-up: mark all tasks as unfinished again
  for (size_t i = 0; i < nsteps; i++)
    steps[i]->done = 0;
}

static int pt2(struct step **steps, const size_t nsteps) {
  #define NWORKERS 5
  int delay_s = 60;
  int worker_finish_times[NWORKERS];
  struct step *worker_tasks[NWORKERS];
  memset(worker_finish_times, 0, sizeof(*worker_finish_times) * NWORKERS);
  memset(worker_tasks, 0, sizeof(struct step *) * NWORKERS);
  size_t ndone = 0;

  // loop over seconds
  for (int s = 0;; s++) {

    // first loop, mark steps as done
    for (size_t w = 0; w < NWORKERS; w++) {
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
    for (size_t w = 0; w < NWORKERS; w++) {
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

int main(void) {
  clock_t t = clock_time();
  char input[1024 * 64] = "";
  read_input_file(input, 1024 * 64, "07.txt");

  struct step steps[64];
  size_t nsteps = 0;

  const char *s = input;
  char name;
  char name_prereq;
  while (*s != 0x0) {
    s = skip("Step ", s);
    name_prereq = *s++;
    s = skip(" must be finished before step ", s);
    name = *s++;
    while (*s != '\n' && *s != 0x0)
      s++;
    if (*s == '\n')
      s++;

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
  for (size_t i = 0; i < nsteps; i++) {
    steps_sorted[i] = &steps[i];
  }
  qsort(steps_sorted, nsteps, sizeof(struct step *), step_compare);

  // solve for pt 1
  char a1[64];
  pt1(a1, steps_sorted, nsteps);

  int a2 = pt2(steps_sorted, nsteps);

  printf("--- %s ---\n", PUZZLE_NAME);
  printf("Part 1: %s\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2fms\n", clock_time_since(t));
  return EXIT_SUCCESS;
}
