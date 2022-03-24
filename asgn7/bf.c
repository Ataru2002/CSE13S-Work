#include "bf.h"
#include "salts.h"
#include "bv.h"
#include "speck.h"
#include <stdlib.h>
#include <stdio.h>

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

BloomFilter *bf_create(uint32_t size) { //create the bloom filter
    BloomFilter *bf
        = (BloomFilter *) malloc(sizeof(BloomFilter)); //allocate memory for the bloom filter
    if (bf) {
        //set the salts accordingly
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
        //create the bit vector within the bloom filter
        bf->filter = bv_create(size);
    }
    return bf;
}

void bf_delete(BloomFilter **bf) {
    if (bf) {
        bv_delete(&((*bf)->filter)); //clear the bit vector first
        free(*bf); //free the bloom fileter
        *bf = NULL;
    }
}

uint32_t bf_size(BloomFilter *bf) { //get the bloom filter's size
    return bv_length((bf->filter));
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
    //hash the word into 3 different keys
    uint32_t key1 = hash(bf->primary, oldspeak);
    uint32_t key2 = hash(bf->secondary, oldspeak);
    uint32_t key3 = hash(bf->tertiary, oldspeak);
    key1 %= bf_size(bf);
    key2 %= bf_size(bf);
    key3 %= bf_size(bf);
    bv_set_bit(bf->filter, key1);
    bv_set_bit(bf->filter, key2);
    bv_set_bit(bf->filter, key3);
    //set all 3 bits at the key's position to 1
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
    //hash the word into 3 different keys
    uint32_t key1 = hash(bf->primary, oldspeak);
    uint32_t key2 = hash(bf->secondary, oldspeak);
    uint32_t key3 = hash(bf->tertiary, oldspeak);
    key1 %= bf_size(bf);
    key2 %= bf_size(bf);
    key3 %= bf_size(bf);
    return (bv_get_bit(bf->filter, key1) && bv_get_bit(bf->filter, key2)
            && bv_get_bit(bf->filter, key3));
    //check if all 3 bit positions are set to 1
}

void bf_print(BloomFilter *bf) { //debugging function
    bv_print(bf->filter);
}
