#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int day25() {
    int32_t card_pubkey = 1965712;
    int32_t door_pubkey = 19072108;
    int64_t value = 1;
    int32_t subject_number = 7;
    int32_t loop_size = 0;
    
    // find lowest loop size
    for (int32_t i=1; loop_size == 0; i++) {
        value = value * subject_number;
        value = value % 20201227;
        if (value == card_pubkey) {
            loop_size = i;
            subject_number = door_pubkey;
        }
    }

    // find encryption key using other public key
    value = 1;
    for (int32_t i = 0; i < loop_size; i++) {
        value = value * subject_number;
        value = value % 20201227;
    }
    printf("%ld\n", value);
    assert(value == 16881444);
    return 0;
}
