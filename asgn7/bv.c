#include "bv.h"
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

BitVector *bv_create(uint32_t length) { //create the bit vector
    BitVector *bits = (BitVector *) malloc(sizeof(BitVector)); //allocate memory for the bit vector
    if (bits) {
        bits->length = length;
        bits->vector = (uint8_t *) calloc(length, sizeof(uint8_t)); //allocate memory for the array
    }
    return bits;
}

void bv_delete(BitVector **bv) {
    if (bv) {
        free((*bv)->vector); //free the bit vector array
        free(*bv); //free the bit vector
        *bv = NULL;
    }
}

uint32_t bv_length(BitVector *bv) { //get the bit vector's size
    return bv->length;
}

bool bv_set_bit(BitVector *bv, uint32_t i) { //set the bit in the bit vector to 1
    if (i >= (bv->length) * 8)
        return false;
    uint32_t pos = i / 8;
    uint32_t val = i % 8;
    bv->vector[pos] |= (1 << val); //set the bit to 1
    return true;
}

bool bv_clr_bit(BitVector *bv, uint32_t i) { //set the bit in the bit vector to 0
    if (i >= (bv->length) * 8)
        return false;
    uint32_t pos = i / 8;
    uint32_t val = i % 8;
    bv->vector[pos] &= ~(1 << val); //set the bit to 0
    return true;
}

bool bv_get_bit(BitVector *bv, uint32_t i) { //get the bit status (0 or 1)
    if (i >= (bv->length) * 8)
        return false;
    uint32_t pos = i / 8;
    uint32_t val = i % 8;
    return ((bv->vector[pos] & (1 << val)) > 0); //get the bit
}

void bv_print(BitVector *bv) { //debugging function
    for (uint32_t i = 0; i < bv->length; i++)
        printf("%u ", bv->vector[i]);
    printf("\n");
}
