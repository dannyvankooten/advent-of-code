#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <err.h>

struct Seat {
    int32_t row;
    int32_t column;
    int32_t seat;
};

int32_t
decode_row(char *v) {
    int32_t l = 0;
    int32_t h = 127;

    for (int8_t i=0; i < 6; i++) {
        int32_t d = ((h+1) - l) / 2;
        if (v[i] == 'F') {
            h -= d;
        } else {
            l += d;
        }
    }
    int32_t row = v[6] == 'F' ? l : h;
    return row;
}

int32_t
decode_column(char *v) {
    int32_t l = 0;
    int32_t h = 7;

    for (int8_t i=0; i < 2; i++) {
        int32_t d = ((h+1) - l) / 2;

        if (v[i] == 'L') {
            h -= d;
        } else {
            l += d;
        }
    }

    int32_t column = v[2] == 'L' ? l : h;
    return column;
}

struct Seat 
decode(char *v) {
    int32_t r = decode_row(v);
    int32_t c = decode_column(v+7);
    struct Seat s = {
        .column = c,
        .row = r,
    };

    s.seat = (r * 8) + c;
    return s;
}

int day5() {
    assert(decode_row("FBFBBFFRLR") == 44);
    assert(decode_column("RLR") == 5);
    assert(decode("FBFBBFFRLR").seat == 357);
    assert(decode("BFFFBBFRRR").seat == 567);
    assert(decode("FFFBBBFRRR").seat == 119);
    assert(decode("BBFFBBFRLL").seat == 820);

    FILE *f = fopen("05.input", "r");
    if (!f) {
        err(EXIT_FAILURE, "error reading input file");
    }
    char buf[BUFSIZ] = {0};

    // find seat with highest seat ID
    struct Seat s;
    int32_t max_seat_id = 0;
    while ((fgets(buf, BUFSIZ, f) != NULL)) {
        s = decode(buf);
        if (s.seat > max_seat_id) {
            max_seat_id = s.seat;
        }
    }
    printf("%d\n", max_seat_id);
    assert(max_seat_id == 933);

    // find our seat (missing from list, not at (unknown) front or back)
    int8_t seats[128][8] = {0};
    fseek(f, 0, SEEK_SET);
    while ((fgets(buf, BUFSIZ, f) != NULL)) {
        s = decode(buf);
        seats[s.row][s.column] = 1;
    }
    fclose(f);    

    int32_t result = 0;
    for (int8_t r=1; r < 127; r++) {
        for (int8_t c=0; c < 8; c++) {
            if (seats[r][c] == 0) {
                result = r * 8 + c;
                goto RESULT_FOUND;
            }
        }
    }

    RESULT_FOUND: ;
    printf("%d\n", result);
    assert(result == 711);
    return 0;
}
