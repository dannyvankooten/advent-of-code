#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <string.h>

struct rule {
    char type[64];
    int range_start;
    int range_end;
};

int
parse_digit(int *d, char *s) {
    char *start = s;
    while (*s == ' ') s++;

    char buf[8];
    char *b = buf;
    while (*s >= '0' && *s <= '9') {
        *b++ = *s++;
    }
    *b = '\0';
    *d = strtol(buf, NULL, 10);
    return s - start;
}

int main() {
    struct rule *rules = malloc(sizeof(struct rule) * 100);
    size_t nrules = 0;
    // read input file
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    char buf[64];
    char *a, *b;

    // parse rules
    while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
        a = linebuf;
        struct rule r;

        // parse type
        b = buf;
        while (*a != ':') {
            *b++ = *a++;
        }
        *b = '\0';
        strcpy(r.type, buf);
        a++;

        a += parse_digit(&r.range_start, a);
        while (*(a-1) != '-') a++;
        a += parse_digit(&r.range_end, a);
        rules[nrules++] = r;
        

        struct rule r2;
        strcpy(r2.type, r.type);
        a = strstr(a, "or") + 2;
        a += parse_digit(&r2.range_start, a);
        while (*(a-1) != '-') a++;
        a += parse_digit(&r2.range_end, a);
        rules[nrules++] = r2;
    }

    // skip foward to line saying "nearby tickets:"
    while (fgets(linebuf, BUFSIZ, f) != NULL && strstr(linebuf, "nearby tickets:") != linebuf);

    // print rules
    for (size_t i=0; i < nrules; i++) {
        printf("%s: %d - %d\n", rules[i].type, rules[i].range_start, rules[i].range_end);
    }

    int sum = 0;
    int d;
    char valid_any;
    while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
        a = linebuf;

        // parse all digits on line
        while (*a != '\n' && *a != '\0') {
            a += parse_digit(&d, a);

            // loop over rules
            // if digit is not in any range, add to sum of invalid values
            valid_any = 0;
            for (size_t i=0; i < nrules; i++) {
                if (d >= rules[i].range_start && d <= rules[i].range_end) {
                    valid_any = 1;
                    break;
                }
            }
            if (0 == valid_any) {
                sum += d;
            }

            if (*a == ',') {
                a++; 
            }
                       
        }
        
    }
    printf("Sum: %d\n", sum);


    fclose(f);

    
}
