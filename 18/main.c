#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <ctype.h>


int tokens_read;

unsigned long long eval(char *input) {
    char *s = input;
    char buf[256];
    buf[0] = '\0';
    char *tmp = buf;
    unsigned long long left, right;
    char op;

    while (isspace(*s)) s++;

    if (*s == '(') {
        s++; // (
        left = eval(s);
        s += tokens_read;
        s++; // )
    } else {
        // parse digit
        while (*s >= '0' && *s <= '9') {
            *tmp++ = *s++;
        }
        *tmp = '\0';
        left = strtol(buf, 0, 10);
        tmp = buf;
    }

    while (*s != '\0') {
        // parse operator
        while (isspace(*s)) s++;
        op = *s++;
        if (op != '+' && op != '*') {
            err(EXIT_FAILURE, "invalid operator: %c", op);
        }
        while (isspace(*s)) s++;

        // parse right
        if (*s == '(') {
            s++; // (
            right = eval(s);
            s += tokens_read;
            s++; // )
        } else {
            // parse digit
            while (*s >= '0' && *s <= '9') {
                *tmp++ = *s++;
            }
            *tmp = '\0';
            right = strtol(buf, 0, 10);
            tmp = buf;
        }
        while (isspace(*s)) s++;
        
        // do operation
        switch (op) {
            case '+':
                left += right;
                break;
            case '*':
                left *= right;
                break;
            // case '-':
            //     left -= right;
            //     break;
            // case '/':
            //     left /= right;
            //     break;
            default:
                err(EXIT_FAILURE, "invalid operator: %c\n", op);
                break;
        }

        if (*s == ')') {
            break;
        }
    }

    tokens_read = s - input;
    return left;
}

int main() {
    assert(eval("1 + 2 * 3 + 4 * 5 + 6") == 71);
    assert(eval("(1 + 2)") == 3);
    assert(eval("((1 + 2) * 3)") == 9);
    assert(eval("1 + 2 * 3") == 9);
    assert(eval("1 + (2 * 3)") == 7);
    assert(eval("1 + (1*(1*(2 * 3)))") == 7);
    assert(eval("2 * 3 + (4 * 5)") == 26);
    assert(eval("1 + (2 * 3) + (4 * (5 + 6))") == 51);
    assert(eval("5 + (8 * 3 + 9 + 3 * 4 * 3)") == 437);
    assert(eval("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 12240);
    assert(eval("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == 13632);

    unsigned long long sum = 0;
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        sum += eval(linebuf);
    }
    fclose(f);

    printf("Result: %lld\n", sum);
}
