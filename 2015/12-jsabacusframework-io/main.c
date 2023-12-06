#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

static inline
char *parse_int(int *dst, char *s) {
    int n = 0;
    int mod = 1;

    if (*s == '-') {
        mod = -1;
        s++;
    }

    while (*s >= '0' && *s <= '9') {
        n = (n * 10) + (*s - '0');
        s++;
    }

    n *= mod;
    *dst = n;

    return s;
}

int pt1(char *input) {
    int sum = 0;
    char *s = input;
    int n;
    while (*s != '\0') {
        if (*s == '-' || (*s >= '0' && *s <= '9')) {
            s = parse_int(&n, s);
            sum += n;
        }

        s++;
    }
    return sum;
}

int pt2(char *input) {
    int sum = 0;
    char *s = input;
    int n;

    int objsums[64];
    memset(objsums, 0, 64 * sizeof(int));
    objsums[0] = 0;
    int objs = 0;
    int red = 0;
    while (*s != '\0') {

        // only add to sum if not in "red" object
        if (red == 0 && (*s == '-' || (*s >= '0' && *s <= '9'))) {
            s = parse_int(&n, s);
            sum += n;
            objsums[objs] += n;
        }

        switch (*s) {
            case '{':
                objs++;
                objsums[objs] = 0;
                break;

            case '}':
                // check if we broke out of red objecty
                if (red == objs) {
                    red = 0;
                }
                objs--;
                break;

            case ':':
                // if property has value of "red"
                // mark current object as red
                // and substract everything added to sum within this object
                // or any of its children
                if (red == 0 && strncmp(s, ":\"red\"", 6) == 0) {
                    red = objs;

                    for (int i=63; i >= objs; i--) {
                        sum -= objsums[i];
                        objsums[i] = 0;
                    }
                }
                break;
        }


        s++;
    }
    return sum;
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    char *input = (char *) malloc(1024 * 32);
    size_t nread = fread(input, 1, 1024*32, fp);
    fclose(fp);
    input[nread] = '\0';

    clock_t start_t, end_t;
    start_t = clock();

    printf("--- Day 12: JSAbacusFramework.io ---\n");
    int a1 = pt1(input);
    printf("Part 1: %d\n", a1);
    assert(a1 == 119433);

    assert(pt2("[1,{\"a\": 10, \"c\":\"red\",\"b\":2},3]") == 4);
    assert(pt2("[1,{\"c\":\"red\",\"b\":2},3]") == 4);
    assert(pt2("{\"d\":\"red\",\"e\":[1,2,3,4],\"f\":5}") == 0);
    assert(pt2("[{\"a\":5,\"color\":\"red\", \"b\": 4}]") == 0);
    assert(pt2("[{\"a\":5,\"b\":\"red\",\"c\":{\"a\": 5}}]") == 0);
    assert(pt2("[{\"a\":5},{\"a\":5,\"b\":\"red\",\"c\":{\"a\": 5}}]") == 5);
    assert(pt2("{\"a\":{\"a\":[1,2,3]}, \"b\":\"red\"}]") == 0);
    assert(pt2("[1,\"red\",5]") == 6);
    assert(pt2("[1,2,3]") == 6);

    int a2 = pt2(input);
    printf("Part 2: %d\n", a2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
    return 0;
}