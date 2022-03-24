#include "randstate.h"
#include <stdlib.h>

gmp_randstate_t state;

void randstate_init(uint64_t seed) {
    mpz_t s;
    mpz_init(s);
    mpz_set_ui(s, seed);
    gmp_randinit_mt(state);
    gmp_randseed(state, s);
    srandom(seed);
    mpz_clear(s);
}

void randstate_clear(void) {
    gmp_randclear(state);
}
