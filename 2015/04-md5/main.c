#include "../adventofcode.h"
#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char *itoa(int value, char *result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) {
    *result = '\0';
    return result;
  }

  char *ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrst"
             "uvwxyz"[35 + (tmp_value - value * base)];
  } while (value);

  // Apply negative sign
  if (tmp_value < 0)
    *ptr++ = '-';
  *ptr-- = '\0';
  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

int main() {
  clock_t start_t = clock_time();
  char input[64 * 1024] = "";
  size_t keylen = read_input_file(input, 64 * 1024, "input.txt");

  EVP_MD_CTX *mdctx;
  mdctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex2(mdctx, EVP_md5(), NULL);

  unsigned char digest[16];
  unsigned char zeroes[] = {0, 0, 0};

  char *dst = input + keylen;
  int i = 1;
  for (;; i++) {
    itoa(i, dst, 10);
    EVP_DigestInit_ex2(mdctx, NULL, NULL);
    EVP_DigestUpdate(mdctx, input, keylen + strlen(dst));
    EVP_DigestFinal_ex(mdctx, digest, NULL);

    if (memcmp(digest, zeroes, 2) == 0 && digest[2] <= 8) {
      break;
    }
  }
  int pt1 = i;

  // continue for part 2
  for (;; i++) {
    itoa(i, dst, 10);
    EVP_DigestInit_ex2(mdctx, NULL, NULL);
    EVP_DigestUpdate(mdctx, input, keylen + strlen(dst));
    EVP_DigestFinal_ex(mdctx, digest, NULL);

    if (memcmp(digest, zeroes, 3) == 0) {
      break;
    }
  }

  EVP_MD_CTX_free(mdctx);

  printf("--- Day 4: The Ideal Stocking Stuffer ---\n");
  printf("Part 1: %d\n", pt1);
  printf("Part 2: %d\n", i);
  printf("Time: %.2f ms\n", clock_time_since(start_t));
  return EXIT_SUCCESS;
}
