#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>

int tokens_read;

typedef struct token {
    enum token_type {
        TOK_NUMBER,
        TOK_PLUS,
        TOK_ASTERISK,
        TOK_LPAREN,
        TOK_RPAREN,
        TOK_EOF
    } type;
    unsigned long long value;
} token_t;

static const char *token_names[] = {
    "NUMBER",
    "PLUS",
    "ASTERISK",
    "LPAREN",
    "RPAREN",
    "EOF",
};

static char *s;
unsigned long long _eval();

token_t
gettoken() {
    while (isspace(*s)) s++;

    switch (*s) {
        case '(':
            s++;
            return (token_t) {
                .type = TOK_LPAREN,
            };
            break;
        case ')':
            s++;
            return (token_t) {
                .type = TOK_RPAREN,
            };
            break;
        case '*':
            s++;
            return (token_t) {
                .type = TOK_ASTERISK,
            };
            break;
        case '+':
            s++;
            return (token_t) {
                .type = TOK_PLUS,
            };
            break;
        case '\n':
        case '\0':
            return (token_t ) {
                .type = TOK_EOF,
            };
            break;
        default:
            if (!isdigit(*s)) {
                break;
            }

            char buf[64];
            char *n = buf;
            while (isdigit(*s)) {
                *n++ = *s++;
            }
            *n = '\0';
            return (token_t) {
                .type = TOK_NUMBER,
                .value = strtoull(buf, NULL, 10),
            };
        break;
    }

    err(EXIT_FAILURE, "Invalid token at '%s'", s);
}

token_t
nexttoken() {
    char *tmp = s;
    token_t tok = gettoken();
    s = tmp;
    return tok;
}

unsigned long long 
_eval_expression(unsigned long long left) {
    unsigned long long right = 0;
    token_t tok;

    while (nexttoken().type != TOK_EOF && *s != '\n') {
        token_t op = gettoken();
        if (op.type != TOK_ASTERISK && op.type != TOK_PLUS) {
            err(EXIT_FAILURE, "expected operator, got %s\n", token_names[op.type]);
        }
        
        tok = gettoken();
        switch (tok.type) {
            case TOK_NUMBER:
                right = tok.value;
                if (op.type == TOK_ASTERISK && nexttoken().type == TOK_PLUS) {
                    right = _eval_expression(right);
                } 
                break;

            case TOK_LPAREN:
                right = _eval();
                break;

            default: 
                err(EXIT_FAILURE, "Invalid token at '%s'. Expected number or grouped expression.", s);
                break;
        }

        switch (op.type) {
            case TOK_ASTERISK:
                left *= right;
                break;

            case TOK_PLUS:
                left += right;
                break;

            default:
                err(EXIT_FAILURE, "Invalid operator");
            break;
        }   

        if (nexttoken().type == TOK_RPAREN) {
            gettoken();
            break;
        }
    }

    return left;
}

unsigned long long _eval() {
    unsigned long long left = 0;

    token_t tok = gettoken();
    switch (tok.type) {
        case TOK_NUMBER:
            left = tok.value;
            break;
        case TOK_LPAREN:
            left = _eval();
            break;
        default:
            err(EXIT_FAILURE, "Invalid token at '%s'. Expected number or group, got %s\n", s, token_names[tok.type]);
            break;
    }

    return _eval_expression(left);
}

unsigned long long eval(char *input) {
    s = input;
    return _eval();
}


int main() {
    assert(eval("1 + 2 * 3 + 4 * 5 + 6") == 231);
    assert(eval("2 * 3 + (4 * 5)") == 46);
    assert(eval("1 + (2 * 3) + (4 * (5 + 6)))") == 51);
    assert(eval("5 + (8 * 3 + 9 + 3 * 4 * 3)") == 1445);
    assert(eval("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 669060);
    assert(eval("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == 23340);
    assert(eval("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == 23340);
    assert(eval("10 * 30 + 50") == 800);
    assert(eval("10 * 30 + (50 * 2)\n") == 1300);
    assert(eval("10 * (30 + (50 * 2))") == 1300);
    assert(eval("(10 * 30) + (50 * 2)") == 400);
    assert(eval("(10 * 30) + 2\n") == 302);



    unsigned long long sum = 0;
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    int l = 0;
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        sum += eval(linebuf);
        l++;
    }
    fclose(f);

    printf("Result: %lld (%d lines)\n", sum, l);
}
