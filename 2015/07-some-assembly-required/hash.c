#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p != '\0'; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

#define HASH(v) ((v[0] - 'a' + 1) * 27 + (v[1] == '\0' ? 0 : v[1] - 'a' + 1))

int main() {
    char buf[3] = { '\0' * 3 };
    printf("buf[3] = %d\n", buf[2]);
    for (char c1 = 'a'; c1 <= 'z'; c1++) {
        buf[0] = c1;
        buf[1] = 0;
        printf("%ld\n", HASH(buf));

        for (char c2 = 'a'; c2 <= 'z'; c2++) {
            buf[0] = c1;
            buf[1] = c2;
            buf[2] = 0;
            printf("%ld\n", HASH(buf));
        }
    }

}