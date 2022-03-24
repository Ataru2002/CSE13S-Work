/*
Author: Nhan Nguyen
Class: CSE13s
*/
#include "numtheory.h"
#include "randstate.h"

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t t;
    mpz_t a2;
    mpz_t b2;
    mpz_init(t);
    mpz_init_set(a2, a);
    mpz_init_set(b2, b);
    while (mpz_cmp_ui(b2, 0) != 0) {
        mpz_set(t, b2);
        mpz_mod(b2, a2, b2);
        mpz_set(a2, t);
    }
    mpz_set(d, a2);
    mpz_clears(t, a2, b2, NULL);
}

void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r;
    mpz_t r2;
    mpz_t t;
    mpz_t t2;
    mpz_t sub1;
    mpz_t sub2;
    mpz_t q;

    mpz_init(r);
    mpz_init(r2);
    mpz_init(t);
    mpz_init(t2);
    mpz_init(sub1);
    mpz_init(sub2);
    mpz_init(q);

    mpz_set(r, n);
    mpz_set(r2, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(t2, 1);
    while (mpz_cmp_ui(r2, 0) != 0) {
        mpz_fdiv_q(q, r, r2);
        mpz_mul(sub1, q, r2);
        mpz_sub(sub1, r, sub1);
        mpz_mul(sub2, q, t2);
        mpz_sub(sub2, t, sub2);

        mpz_set(r, r2);
        mpz_set(r2, sub1);

        mpz_set(t, t2);
        mpz_set(t2, sub2);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(i, 0);
        return;
    }
    if (mpz_cmp_ui(t, 0) < 0)
        mpz_add(t, t, n);
    mpz_set(i, t);
    mpz_clears(r, r2, t, t2, sub1, sub2, q, NULL);
}

void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t v;
    mpz_t p;
    mpz_t m;
    mpz_t e2;

    mpz_init(v);
    mpz_init(p);
    mpz_init(m);
    mpz_init_set(e2, exponent);

    mpz_set_ui(v, 1);
    mpz_set(p, base);
    while (mpz_cmp_ui(e2, 0) > 0) {
        mpz_mod_ui(m, e2, 2);
        if (mpz_cmp_ui(m, 0) != 0) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, modulus);
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, modulus);
        mpz_fdiv_q_ui(e2, e2, 2);
    }
    mpz_set(out, v);
    mpz_clears(v, p, m, e2, NULL);
}

bool is_prime(mpz_t n, uint64_t iters) {
    //if (mpz_even_p(n) != 0)
    //    return false;
    if (mpz_cmp_ui(n, 1) <= 0)
        return false;
    mpz_t r;
    mpz_t s;
    mpz_t a;
    mpz_t y;
    mpz_t j;
    mpz_t sub3;
    mpz_t high;
    mpz_t sub;
    mpz_t sub2;
    mpz_t sub4;

    mpz_init(r);
    mpz_init(s);
    mpz_init(a);
    mpz_init(y);
    mpz_init(j);
    mpz_init(sub3);
    mpz_init(high);
    mpz_init(sub);
    mpz_init(sub2);
    mpz_init(sub4);

    mpz_sub_ui(r, n, 1);
    mpz_set_ui(s, 0);
    mpz_mod_ui(sub3, r, 2);
    while (mpz_cmp_ui(sub3, 0) == 0) {
        mpz_fdiv_q_ui(r, r, 2);
        mpz_add_ui(s, s, 1);
        mpz_mod_ui(sub3, r, 2);
    }

    for (uint64_t i = 0; i < iters; i++) {
        mpz_sub_ui(high, n, 3);
        mpz_sub_ui(sub, n, 1);
        mpz_sub_ui(sub2, s, 1);
        mpz_set_ui(sub4, 2);
        mpz_urandomm(a, state, high);
        mpz_add_ui(a, a, 2);

        pow_mod(y, a, r, n);
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, sub) != 0) {
            mpz_set_ui(j, 1);
            while (mpz_cmp(j, sub2) <= 0 && mpz_cmp(y, sub) != 0) {
                pow_mod(y, y, sub4, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(r, s, a, y, j, sub3, high, sub, sub2, sub4, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, sub) != 0) {
                mpz_clears(r, s, a, y, j, sub3, high, sub, sub2, sub4, NULL);
                return false;
            }
        }
    }
    mpz_clears(r, s, a, y, j, sub3, high, sub, sub2, sub4, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t gen;
    mpz_t mx;
    mpz_t low;
    mpz_t bin;
    mpz_init(gen);
    mpz_init(mx);
    mpz_init(low);
    mpz_init(bin);
    mpz_set_ui(bin, 2);
    mpz_pow_ui(low, bin, bits);
    while (true) {
        mpz_urandomb(gen, state, bits);
        mpz_add(gen, gen, low);
        if (is_prime(gen, iters))
            break;
    }
    mpz_set(p, gen);
    mpz_clears(gen, mx, low, bin, NULL);
}
