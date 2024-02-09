#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "intmap.h"

static int64_t
modpow(int64_t x, int exp, int mod)
{
    int64_t res = 1;
    x = x % mod;

    while (exp > 0)
    {
        // If exp is odd, multiply x with result
        if (exp & 1) {
            res = (res * x) % mod;
        }

        // exp must be even now
        exp = exp >> 1;
        x = (x * x) % mod;
    }

    return res;
}

static int
discrete_log(int subject, int value, int mod) {
  int e = 1;
  int modw = (int) sqrt((double) mod);
  intmap_t *hm = intmap_new(modw);

  for (int i=0; i < modw; i++) {
    intmap_set(hm, e, i);
    e = (e * subject) % mod;
  }
  int64_t f = modpow(subject, mod-modw-1, mod);

  // baby step giant step
  int r;
  for(int i=0; i < modw; i++) {
    r = intmap_get(hm, value);
    if (r != 0) {
      intmap_free(hm);
      return i * modw + r;
    }

    value = (value * f) % mod;
  }

  exit(1);
}

int day25(void) {
  int card_pubkey = 1965712;
  int door_pubkey = 19072108;
  int loop_size = discrete_log(7, card_pubkey, 20201227);
  assert(loop_size == 7779516);

  // find encryption key using other public key
  int64_t value = modpow(door_pubkey, loop_size, 20201227);
  assert(value == 16881444);
  printf("%ld\n", value);
  return 0;
}
