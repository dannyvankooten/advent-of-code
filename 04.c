#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>


// https://adventofcode.com/2020/day/4#part2

char 
is_passport_valid(char passport[]) {
    static const char valid_1[] = {1, 1, 1, 1, 1, 1, 1, 1};
    if (memcmp(passport, valid_1, 8 * sizeof(char)) == 0) {
        return 1;
    }

    static const char valid_2[] = {1, 1, 1, 1, 1, 1, 1, 0};
    return memcmp(passport, valid_2, 8 * sizeof(char)) == 0;
}

char
is_valid_hgt(char *v) {
    int32_t n = 0;
    int32_t i = 0;

    while (*v >= '0' && *v <= '9') {
        n = n * 10 + (*v - '0');
        i++;
        v++;
    }
    if (i == 0 || i > 3) {
        return 0;
    }

    if (strcmp(v, "cm") == 0) {
        return n >= 150 && n <= 193;
    }

    if (strcmp(v, "in") == 0) {
        return n >= 59 && n <= 76;
    }

    return 0;
} 

char
is_valid_hcl(char *v) {
   if (v[0] != '#') {
        return 0;
    }
    
    for (int i=1; i < 7; i++) {
        if (v[i] == '\0') {
            return 0;
        }

        if (!('a' <= v[i] && v[i] <= 'f')
            && !('0' <= v[i] && v[i] <= '9')) {
                return 0;    
        }
    }

    if (v[7] != '\0') {
        return 0;
    }

    return 1;
} 

char
is_valid_ecl(char *v) {
    static const char *valid_values[] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };

    for (int i=0; i < 7; i++) {
        if (strcmp(v, valid_values[i]) == 0) {
            return 1;
        }
    }

    return 0;
} 

char
is_valid_pid(char *v) {
    for (int i=0; i < 9; i++) {
        if (v[i] < '0' || v[i] > '9' || v[i] == '\0') {
            return 0;
        }
    }

    if (v[9] != '\0') {
        return 0;
    }

    return 1;
} 

enum field_key {
    BYR = 0,
    IYR,
    EYR,
    HGT,
    HCL,
    ECL,
    PID,
    CID
};

int 
day4() {
	FILE *f = fopen("04.input", "r");
    if (!f) {
        return 1;
    }
    char buf[BUFSIZ];
    char fields[8] = {0};
    char key[32];
    char value[32];
    size_t i;
    int64_t valid_pp_count = 0;
    int64_t v;

    while (fgets(buf, BUFSIZ, f) != NULL) {
        char *str = buf;

        // blank line, new passport
        if (*buf == '\n') {
            if (is_passport_valid(fields)) {
                valid_pp_count++;
            }
            memset(fields, 0, 8 * sizeof(char));
            continue;
        }
        
        while (*str != '\n' && *str != '\0') {
            // parse key
            for (i=0; *str != ':'; i++) {
                key[i] = *str++;
            }
            key[i] = '\0';

            // skip :
            str++;

            // parse value (or skip for now)
            for (i=0; *str != ' ' && *str != '\n'; i++) {
                value[i] = *str++;
            }
            value[i] = 0;

            // skip spaces
            while (*str == ' ') {
                str++;
            }

            // add to fields and then check validity
            if (strcmp(key, "byr") == 0) {
                v = strtol(value, 0, 10);
                if (v >= 1920 && v <= 2002) {
                    fields[BYR] = 1;
                }
            } else if(strcmp(key, "iyr") == 0) {
                v = strtol(value, 0, 10);
                if (v >= 2010 && v <= 2020) {
                    fields[IYR] = 1;
                }
            } else if(strcmp(key, "eyr") == 0) {
                v = strtol(value, 0, 10);
                if (v >= 2020 && v <= 2030) {
                    fields[EYR] = 1;
                }
            } else if(strcmp(key, "hgt") == 0) {
                fields[HGT] = is_valid_hgt(value);
            } else if (strcmp(key, "hcl") == 0) {
                fields[HCL] = is_valid_hcl(value);
            } else if(strcmp(key, "ecl") == 0) {
                fields[ECL] = is_valid_ecl(value);
            } else if (strcmp(key, "pid") == 0) {
                fields[PID] = is_valid_pid(value);
            } else if (strcmp(key, "cid") == 0) {
                fields[CID] = 1;
            }
        }
    }

    // last line
    if (is_passport_valid(fields)) {
        valid_pp_count++;
    }

    printf("%" PRId64 "\n", valid_pp_count);
    fclose(f);
    return 0;
}
