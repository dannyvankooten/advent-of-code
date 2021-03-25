#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

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
    long long value;
    int precedence;
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
long long _eval();

enum precedence {
    LOWEST = 1,
    PRODUCT,
    SUM,
    CALL,
};

token_t
gettoken() {
    while (*s == ' ') s++;

    token_t tok = {
        .precedence = LOWEST,
    };

    switch (*s) {
        case '(':
            s++;
            tok.type = TOK_LPAREN;
            tok.precedence = CALL;
            break;
        case ')':
            s++;
            tok.type = TOK_RPAREN;
            break;
        case '*':
            s++;
            tok.type = TOK_ASTERISK;
            tok.precedence = PRODUCT;
            break;
        case '+':
            s++;
            tok.type = TOK_PLUS;
            tok.precedence = SUM;
            break;
        case '\n':
        case '\0':
            tok.type = TOK_EOF;
            break;
        default:
            if (!isdigit(*s)) {
                err(EXIT_FAILURE, "Invalid token at '%s'", s);
            }

            char buf[64];
            char *n = buf;
            while (isdigit(*s)) {
                *n++ = *s++;
            }
            *n = '\0';
            tok.type = TOK_NUMBER;
            tok.value = strtoll(buf, NULL, 10);
        break;
    }

    return tok;
}

token_t
nexttoken() {
    char *tmp = s;
    token_t tok = gettoken();
    s = tmp;
    return tok;
}

long long 
_eval_infix_expression(long long left) {
    // parse operator
    token_t op = gettoken();
    if (op.type != TOK_ASTERISK && op.type != TOK_PLUS) {
        err(EXIT_FAILURE, "expected operator, got %s\n", token_names[op.type]);
    }

    // evaluate right
    long long right = _eval(op.precedence);

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

    return left;
}



long long _eval(int precedence) {
    long long left;

    token_t tok = gettoken();
    switch (tok.type) {
        case TOK_NUMBER:
            left = tok.value;
            break;
        case TOK_LPAREN:
            left = _eval(0);

            // expect RPAREN
            if (nexttoken().type != TOK_RPAREN) {
                err(EXIT_FAILURE, "Invalid token at '%s'. Expected RPAREN.", s);
            } else {
                gettoken();
            }
            break;
        default:
            err(EXIT_FAILURE, "Invalid token at '%s'. Expected number or group, got %s\n", s, token_names[tok.type]);
            break;
    }

    while (nexttoken().type != TOK_EOF && precedence < nexttoken().precedence) {
        if (nexttoken().type == TOK_RPAREN) {
            return left;
        }

        left = _eval_infix_expression(left);
    }

    return left;
}

long long eval(char *input) {
    s = input;
    return _eval(1);
}


int main() {
    assert(eval("1 + 2 * 3 + 4 * 5 + 6") == 231);
    assert(eval("2 * 3 + (4 * 5)") == 46);
    assert(eval("1 + (2 * 3) + (4 * (5 + 6)))") == 51);
    assert(eval("5 + (8 * 3 + 9 + 3 * 4 * 3)") == 1445);
    assert(eval("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))") == 669060);
    assert(eval("2 * (9 * 8) + 6") == 156);
    assert(eval("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2") == 23340);
    assert(eval("(((2 + 4) * 9) * ((6 + 9) * 8 + 6) + 6) + (2 + 4) * 2") == 23340);
    assert(eval("9 + 3 * ((2 * 7) * 4 + 9 + 8 * (6 + 5) * 7) * 5 + 9") == 3803184);
    assert(eval("2 * 6 * 2 + 2 * 8 * (3 + 2 * (9 * 8) + 6 + 9 + 2)") == 170880);

    long long sum = 0LL;
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        sum += eval(linebuf);
    }
    fclose(f);

    printf("Result: %lld\n", sum);
}
