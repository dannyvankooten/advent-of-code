#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <ctype.h>

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
    int value;
} token_t;

char *token_names[] = {
    "NUMBER",
    "PLUS",
    "ASTERISK",
    "LPAREN",
    "RPAREN",
    "EOF",
};

char *s;

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
            *n++ = '\0';
            return (token_t) {
                .type = TOK_NUMBER,
                .value = strtol(buf, NULL, 10),
            };
        break;
    }

    err(EXIT_FAILURE, "Unknown token at '%s'", s);
}

token_t
nexttoken() {
    char *tmp = s;
    token_t tok = gettoken();
    s = tmp;
    return tok;
}

void
expect(enum token_type type) {
    token_t tok = gettoken();
    if (tok.type != type) {
        err(EXIT_FAILURE, "expected token of type %s, got %s", token_names[type], token_names[tok.type]);
    }
}

unsigned long long _eval() {
    unsigned long long left = 0;
    unsigned long long right = 0;

    token_t tok = gettoken();
    switch (tok.type) {
        case TOK_NUMBER:
            left = tok.value;
            break;
        case TOK_LPAREN:
            left = _eval();
            break;
        default:
            err(EXIT_FAILURE, "Invalid token: %s\n", token_names[tok.type]);
            break;
    }


    while (tok.type != TOK_EOF && nexttoken().type != TOK_EOF) {
        token_t op = gettoken();
        if (op.type != TOK_ASTERISK && op.type != TOK_PLUS) {
            err(EXIT_FAILURE, "expected operator, got %s\n", token_names[op.type]);
        }
        
        tok = gettoken();
        switch (tok.type) {
            case TOK_NUMBER:
                right = tok.value;
                break;

            case TOK_LPAREN:
                right = _eval();
                break;

            default: break;
        }

        switch (op.type) {
            case TOK_ASTERISK:
                left *= right;
                break;

            case TOK_PLUS:
                left += right;
                break;

            default: break;
        }   

        if (nexttoken().type == TOK_RPAREN) {
            gettoken();
            return left;
        }
    }


    return left;
}

unsigned long long eval(char *input) {
    s = input;
   return _eval();
}


int main() {
    assert(eval("1 + 2 * 3 + 4 * 5 + 6") == 71);
    assert(eval("2 * 3 + (4 * 5)") == 26);
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
