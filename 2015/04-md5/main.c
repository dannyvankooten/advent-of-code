#include <openssl/md5.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main() {
    unsigned char digest[16];
    const char *key = "yzbqklnj";
    int keylen = strlen(key);
    char *input = (char *) malloc(128);
    memcpy(input, key, keylen);
    input[keylen] = '\0';
    printf("input: '%s'\n", input);

    int i = 1;
    int pt1 = 0;
    int pt2 = 0;
    MD5_CTX context;

    for (;; i++) {
        sprintf(input + keylen, "%d", i);

        MD5_Init(&context);
        MD5_Update(&context, input, strlen(input));
        MD5_Final(digest, &context);

        if (digest[0] == 0 && digest[1] == 0 && digest[2] <= 8) {
            break;
        }
    }

    printf("part 1: %d\n", i);

    unsigned char zeroes[] = {0, 0, 0};

    // continue for part 2
    for (;; i++) {
        sprintf(input + keylen, "%d", i);

        MD5_Init(&context);
        MD5_Update(&context, input, strlen(input));
        MD5_Final(digest, &context);

        if (memcmp(digest, zeroes, 3) == 0) {
            break;
        }
    }
    printf("part 2: %d\n", i);
    return 0;
}
