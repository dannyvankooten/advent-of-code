#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

typedef struct {
    int64_t pubkey;
    int64_t loop_size;
} agent_t;

int main() {
   agent_t door = {
    //    .pubkey = 17807724,
       .pubkey = 19072108,
       .loop_size = 0,
   };
   agent_t card = {
    //    .pubkey = 5764801,
       .pubkey = 1965712,
       .loop_size = 0,
   };

   int64_t value = 1;
   int64_t subject_number = 7;
    
   for (int64_t i=1; door.loop_size == 0 || card.loop_size == 0; i++) {
       value = value * subject_number;
       value = value % 20201227;
       if (value == card.pubkey) {
           printf("Card loop size: %ld\n", i);
           card.loop_size = i;          
       } else if (value == door.pubkey) {
           printf("Door loop size: %ld\n", i);
           door.loop_size = i;
       }
   }

   value = 1;
   subject_number = card.pubkey;
   for (int i = 0; i < door.loop_size; i++) {
       value = value * subject_number;
       value = value % 20201227;
   }
   printf("Encryption key: %ld\n", value);
}
