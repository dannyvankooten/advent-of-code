#include <stdio.h>
#include <time.h>
#include <string.h>
#include <openssl/md5.h>
#include <stdint.h>

void solve(char *doorid, char *password, char *password2) {
    int p1 = 0;
    int p2 = 0;
    int pos;
    strcpy(password2, "........");
    int keylen = strlen(doorid);
    uint8_t input[32] = "abbhdwsy";
    uint8_t digest[MD5_DIGEST_LENGTH];

    char buf[8];
    char *dst = input + keylen;
    int i = 1;
    for (;; i++) {
        sprintf(dst, "%d", i);
        MD5(input, keylen + strlen(dst), digest);

        unsigned char skip = digest[0] | digest[1] | (digest[2] & 0xf0);
        if (!skip) {
            sprintf(buf, "%02x%02x", digest[2], digest[3]);

            if (p1 < 8) {
                password[p1++] = buf[1];
            }

            pos = buf[1] - '0';
            if (pos >= 0 && pos < 8 && password2[pos] == '.') {
                password2[pos] = buf[2];
                p2++;
            }

            if (p1 >= 8 && p2 >= 8) {
                break;
            }
        }
    }

//    EVP_MD_CTX_free(mdctx);
}


int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char pt1buf[16];
    char pt2buf[16];
    solve("abbhdwsy", pt1buf, pt2buf);

    printf("--- Day 5: How About a Nice Game of Chess? ---\n");
    printf("Part 1: %s\n", pt1buf);
    printf("Part 2: %s\n", pt2buf);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);
}
