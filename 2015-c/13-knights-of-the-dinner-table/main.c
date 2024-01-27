#include "../adventofcode.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

int idx(char seats[32][32], int nseats, char *needle) {
  for (int i = 0; i < nseats; i++) {
    if (strcmp(seats[i], needle) == 0) {
      return i;
    }
  }
  return -1;
}

int parse(const char *s, int matrix[32][32], char seats[32][32]) {
  memset(matrix, 0, 32 * 32 * sizeof(**matrix));
  int nseats = 0;
  char self[32];
  char other[32];
  char buf[32];
  int n = 0;
  while (*s != 0x0) {
    s = parse_ident(self, s);
    s = skip(" would ", s);
    s = parse_ident(buf, s);
    s = skip(" ", s);
    s = parse_int(&n, s);
    s = skip(" happiness units by sitting next to ", s);
    s = parse_ident(other, s);
    s = skip(".", s);
    s = skip_optional('\n', s);
    if (strcmp(buf, "lose") == 0) {
      n *= -1;
    }

    int a = idx(seats, nseats, self);
    if (a == -1) {
      a = nseats;
      strcpy(seats[nseats++], self);
    }
    int b = idx(seats, nseats, other);
    if (b == -1) {
      b = nseats;
      strcpy(seats[nseats++], other);
    }

    matrix[a][b] = n;
  }

  return nseats;
}

#ifdef NDEBUG
static void print_matrix(int matrix[32][32], int n) {
  for (int a = 0; a < n; a++) {
    for (int b = 0; b < n; b++) {
      printf("%d\t", matrix[a][b]);
    }
    printf("\n");
  }
}
#endif

int permute(int matrix[32][32], int buf[], int from, int to, int score) {
  if (from == to) {
    int h = 0;
    for (int i = 0; i < to; i++) {
      int u = buf[i];
      int v = (i == to - 1) ? buf[0] : buf[i + 1];
      h += matrix[u][v] + matrix[v][u];
    }
    return h > score ? h : score;
  }

  int tmp = buf[from];

  for (int i = from; i < to; i++) {
    buf[from] = buf[i];
    buf[i] = tmp;
    score = permute(matrix, buf, from + 1, to, score);
    buf[i] = buf[from];
    buf[from] = tmp;
  }

  return score;
}

int main(void) {
  clock_t tstart = clock_time();
  char input[1024 * 64] = "";
  read_input_file(input, 1024 * 64, "input.txt");
  int a1 = 0;
  int a2 = 0;
  int matrix[32][32];
  char seats[32][32];
  int nseats = parse(input, matrix, seats);

  // create buf with combinations
  int buf[nseats + 1];
  for (int i = 0; i < nseats; i++) {
    buf[i] = i;
  }
  a1 = permute(matrix, buf, 1, nseats, 0);

  // add self
  buf[nseats] = nseats;
  nseats++;
  a2 = permute(matrix, buf, 1, nseats, 0);

  printf("--- Day 13: Knights of the Dinner Table ---\n");
  printf("Part 1: %d\n", a1);
  printf("Part 2: %d\n", a2);
  printf("Time: %.2f ms\n", clock_time_since(tstart));
  return 0;
}
