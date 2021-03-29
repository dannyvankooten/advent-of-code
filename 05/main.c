#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Seat {
    int row;
    int column;
    int seat;
};

int decode_row(char *v) {
    int l = 0;
    int h = 127;

    for (int i=0; i < 6; i++) {
        int d = ((h+1) - l) / 2;
        if (v[i] == 'F') {
            h -= d;
        } else {
            l += d;
        }
    }
    int row = v[6] == 'F' ? l : h;
    return row;
}

int decode_column(char *v) {
    int l = 0;
    int h = 7;

    for (int i=0; i < 2; i++) {
        int d = ((h+1) - l) / 2;

        if (v[i] == 'L') {
            h -= d;
        } else {
            l += d;
        }
    }

    int column = v[2] == 'L' ? l : h;
    return column;
}

struct Seat 
decode(char *v) {
    int r = decode_row(v);
    int c = decode_column(v+7);
    struct Seat s = {
        .column = c,
        .row = r,
    };

    s.seat = (r * 8) + c;
    return s;
}

int main() {
    assert(decode_row("FBFBBFFRLR") == 44);
    assert(decode_column("RLR") == 5);
    assert(decode("FBFBBFFRLR").seat == 357);
    assert(decode("BFFFBBFRRR").seat == 567);
    assert(decode("FFFBBBFRRR").seat == 119);
    assert(decode("BBFFBBFRLL").seat == 820);

    FILE *f = fopen("input.txt", "r");
    if (!f) {
        return 1;
    }
    char buf[BUFSIZ] = {0};

    // find seat with highest seat ID
    struct Seat s;
    int max_seat_id = 0;
    while ((fgets(buf, BUFSIZ, f) != NULL)) {
        s = decode(buf);
        if (s.seat > max_seat_id) {
            max_seat_id = s.seat;
        }
    }
    printf("Part 1: %d\n", max_seat_id);

    // find our seat (missing from list, not at (unknown) front or back)
    int seats[128][8] = {0};
    fseek(f, 0, SEEK_SET);
    while ((fgets(buf, BUFSIZ, f) != NULL)) {
        s = decode(buf);
        seats[s.row][s.column] = 1;
    }
    fclose(f);    

    for (int r=1; r<127; r++) {
        for (int c=0; c<8; c++) {
            if (seats[r][c] == 0) {
                printf("Part 2: Seat %d (%d, %d)\n", r*8+c, r, c);
                return 0;
            }
        }
    }


    
    
}
