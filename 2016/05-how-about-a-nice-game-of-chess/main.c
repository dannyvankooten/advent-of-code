#include <stdio.h>
#include <time.h>
#include <string.h>
#include <openssl/evp.h>

void solve(char *doorid, char *password, char *password2) {
    unsigned char digest[16];
    int p1 = 0;
    int p2 = 0;
    int pos = 0;
    strcpy(password2, "........");
    int keylen = strlen(doorid);
    char input[32];
    strcpy(input, doorid);
    unsigned char zero[2] = {0, 0};

    EVP_MD_CTX *mdctx;
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex2(mdctx, EVP_md5(), NULL);

    char buf[8];
    char *dst = input + keylen;
    int i = 1;
    for (;; i++) {
        sprintf(dst, "%d", i);
        EVP_DigestInit_ex2(mdctx, NULL, NULL);
        EVP_DigestUpdate(mdctx, input, keylen + strlen(dst));
        EVP_DigestFinal_ex(mdctx, digest, NULL);

        if (memcmp(digest, zero, 2) == 0 && digest[2] <= 1 << 4) {
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

    EVP_MD_CTX_free(mdctx);
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
