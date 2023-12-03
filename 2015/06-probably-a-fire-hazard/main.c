#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdint.h>

static inline
char *parse_int(uint16_t *dst, char *s) {
    uint16_t n = 0;
    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }

    *dst = n;
    return s;
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    char input[20*1024];
    size_t nread = fread(input, 1, 20*1024, fp);
    input[nread] = '\0';
    fclose(fp);

    uint8_t grid_pt1[1000 * 1000] = {0};
    uint16_t grid_pt2[1000 * 1000] = {0};
    int8_t action;
    uint16_t x_start;
    uint16_t y_start;
    uint16_t x_end;
    uint16_t y_end;

    char *s = input;
    while (*s != '\0') {
        if (memcmp(s, "toggle", 6) == 0) {
            action = -1;
            s += 7;
        } else if(memcmp(s+5, "on", 2) == 0) {
            action = 1;
            s += 8;
        } else if (memcmp(s+5, "off", 3) == 0) {
            action = 0;
            s += 9;
        } else {
            printf("unexpected input at %s\n", s);
            abort();
        }

        // parse start coords
        s = parse_int(&x_start, s);
        s++; // skip comma
        s = parse_int(&y_start, s);

        // skip " through "
        s += 9;

        // parse end coords
        s = parse_int(&x_end, s);
        s++; // skip comma
        s = parse_int(&y_end, s);

        // skip newline
        s++;

        // update grids
        for (uint16_t y = y_start; y <= y_end; y++) {
            for (uint16_t x = x_start; x <= x_end; x++) {
                uint32_t idx = y * 1000 + x;
                switch (action) {
                    case 0:
                        grid_pt1[idx] = 0;
                        if (grid_pt2[idx] > 0) {
                            grid_pt2[idx]--;
                        }
                        break;
                    case 1:
                        grid_pt1[idx] = 1;
                        grid_pt2[idx] += 1;
                    break;

                    case -1:
                        grid_pt1[idx] = grid_pt1[idx] == 1 ? 0 : 1;
                        grid_pt2[idx] += 2;
                        break;
                }
            }
        }
    }

    // we loop separately here because it's faster (because of cpu cache hits)
    int pt1 = 0;
    for (int i = 0; i < 1000 * 1000; i++) {
        if (grid_pt1[i] == 1) {
            pt1++;
        }
    }
    printf("%d\n", pt1);

    long pt2 = 0;
    for (int i = 0; i < 1000 * 1000; i++) {
        pt2 += grid_pt2[i];
    }
    printf("%ld\n", pt2);


    return 0;
}
