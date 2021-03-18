#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

// https://adventofcode.com/2020/day/4#part2

char 
is_passport_valid(char passport[]) {
    const char valid_1[] = {1, 1, 1, 1, 1, 1, 1, 1};
    if (memcmp(passport, valid_1, 8 * sizeof(char)) == 0) {
        return 1;
    }

    const char valid_2[] = {1, 1, 1, 1, 1, 1, 1, 0};
    return memcmp(passport, valid_2, 8 * sizeof(char)) == 0;
}

char
is_valid_hgt(char *v) {
    long n = 0;
    char buf[3];
    int i = 0;
    while (*v >= '0' && *v <= '9') {
        if (i > 2) {
            return 0;
        }

        buf[i++] = *v;
        v++;
    }
    if (i == 0) {
        return 0;
    }
    buf[i] = '\0';
    n = strtol(buf, 0, 10);

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
    char *valid_values[] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
    int l = sizeof(valid_values) / sizeof(valid_values[0]);

    for (int i=0; i < l; i++) {
        if (strcmp(v, valid_values[i]) == 0) {
            return 1;
        }
    }

    return 0;
} 

char
is_valid_pid(char *v) {
    for (int i=0; i < 9; i++) {
        if (v[i] < '0' || v[i] > '9') {
            return 0;
        }
    }

    if (v[9] != '\0') {
        return 0;
    }

    return 1;
} 

int 
main() {
	FILE *f = fopen("input.txt", "r");
    if (!f) return 1;

    char buf[BUFSIZ];
    char fields[8];
    memset(fields, 0, 8 * sizeof(char));
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
    char key[32];
    char value[32];
    int i;
    int valid_pp_count = 0;
    long v;

    while (fgets(buf, BUFSIZ, f) != NULL) {
        char *str = buf;

        // blank line, new passport
        if (*buf == '\n') {
            if (is_passport_valid(fields)) {
                valid_pp_count++;
            }
            memset(fields, 0, 8 * sizeof(char));
            printf("\n");
            continue;
        }
        
        while (*str != '\n') {
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

    printf("%d\n", valid_pp_count);
    fclose(f);
}
