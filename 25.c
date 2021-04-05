#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hm_int.h"

int64_t 
modpow(int64_t x, int32_t exp, int32_t mod) 
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

int32_t 
discrete_log(int32_t subject, int32_t value, int32_t mod) {
  int32_t e = 1;
  int32_t modw = (int32_t) sqrt((double) mod);
  hashmap_t *hm = hm_new(modw);

  for (int32_t i=0; i < modw; i++) {
    hm_set(hm, e, i);
    e = (e * subject) % mod;
  }
  int64_t f = modpow(subject, mod-modw-1, mod);

  // baby step giant step
  int32_t r;
  for(int32_t i=0; i < modw; i++) {
    r = hm_get(hm, value);
    if (r != 0) {
      free(hm->entries);
      free(hm);
      return i * modw + r;
    }

    value = (value * f) % mod;
  }

  exit(1);
}

// TODO: Look at https://en.wikipedia.org/wiki/Discrete_logarithm#Algorithms 
int day25() {
  int32_t card_pubkey = 1965712;
  int32_t door_pubkey = 19072108;

  int32_t loop_size = discrete_log(7, card_pubkey, 20201227);
  assert(loop_size == 7779516);

  // find encryption key using other public key
  int64_t value = modpow(door_pubkey, loop_size, 20201227);
  assert(value == 16881444);
  printf("%ld\n", value);
  return 0;
}
