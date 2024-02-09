#include <assert.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Seat {
  int row;
  int column;
  int seat;
};

static
int decode_row(const char* v) {
  int l = 0;
  int h = 127;

  for (int8_t i = 0; i < 6; i++) {
    int d = ((h + 1) - l) / 2;
    if (v[i] == 'F') {
      h -= d;
    } else {
      l += d;
    }
  }
  int row = v[6] == 'F' ? l : h;
  return row;
}

static
int decode_column(const char* v) {
  int l = 0;
  int h = 7;

  for (int8_t i = 0; i < 2; i++) {
    int d = ((h + 1) - l) / 2;

    if (v[i] == 'L') {
      h -= d;
    } else {
      l += d;
    }
  }

  int column = v[2] == 'L' ? l : h;
  return column;
}

static
struct Seat decode(const char* v) {
  int r = decode_row(v);
  int c = decode_column(v + 7);
  struct Seat s = {
      .column = c,
      .row = r,
  };

  s.seat = (r * 8) + c;
  return s;
}

int day5(void) {
  assert(decode_row("FBFBBFFRLR") == 44);
  assert(decode_column("RLR") == 5);
  assert(decode("FBFBBFFRLR").seat == 357);
  assert(decode("BFFFBBFRRR").seat == 567);
  assert(decode("FFFBBBFRRR").seat == 119);
  assert(decode("BBFFBBFRLL").seat == 820);

  FILE* f = fopen("inputs/05.txt", "r");
  if (!f) {
    err(EXIT_FAILURE, "error reading input file");
  }
  char buf[BUFSIZ] = {0};

  // find seat with highest seat ID
  struct Seat s;
  int max_seat_id = 0;
  int8_t seats[128][8] = {0};
  while ((fgets(buf, BUFSIZ, f) != NULL)) {
    s = decode(buf);
    if (s.seat > max_seat_id) {
      max_seat_id = s.seat;
    }

    // mark seat as occupied for pt 2
    seats[s.row][s.column] = 1;
  }
  fclose(f);
  printf("%d\n", max_seat_id);
  assert(max_seat_id == 933);

  // find our seat (missing from list, not at (unknown) front or back)
  int result = 0;
  for (int8_t r = 10; r < 127; r++) {
    for (int8_t c = 0; c < 8; c++) {
      if (seats[r][c] == 0) {
        result = r * 8 + c;
        printf("%d\n", result);
        assert(result == 711);
        return 0;
      }
    }
  }

  return 1;
}
