/*
Author: Nhan Nguyen
Class: CSE13s
*/
#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"

#include <stdlib.h>

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    uint64_t pbits = (random() % (((3 * nbits / 4) - (nbits / 4) + 1) + (nbits / 4)));
    uint64_t qbits = nbits - pbits;
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);
    mpz_mul(n, p, q);

    mpz_t p2;
    mpz_t q2;
    mpz_t lcm;
    mpz_t delcm;
    mpz_t loop;

    mpz_init_set(p2, p);
    mpz_init_set(q2, q);
    mpz_init(lcm);
    mpz_init(delcm);
    mpz_init(loop);

    mpz_sub_ui(p2, p, 1);
    mpz_sub_ui(q2, q, 1);
    mpz_mul(lcm, p2, q2);
    gcd(delcm, p2, q2);
    mpz_fdiv_q(lcm, lcm, delcm);

    mpz_urandomb(e, state, nbits);
    gcd(loop, lcm, e);

    while (mpz_cmp_ui(loop, 1) != 0) {
        mpz_urandomb(e, state, nbits);
        gcd(loop, e, lcm);
    }
    mpz_clears(p2, q2, lcm, delcm, loop, NULL);
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    gmp_fprintf(pbfile, "%s\n", username);
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    gmp_fscanf(pbfile, "%s\n", username);
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t p2, q2, lcm, delcm;
    mpz_init(p2);
    mpz_init(q2);
    mpz_init(lcm);
    mpz_init(delcm);

    mpz_sub_ui(p2, p, 1);
    mpz_sub_ui(q2, q, 1);
    mpz_mul(lcm, p2, q2);
    gcd(delcm, p2, q2);
    mpz_fdiv_q(lcm, lcm, delcm);

    mod_inverse(d, e, lcm);
    mpz_clears(p2, q2, lcm, delcm, NULL);
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    mpz_t convert;
    mpz_init(convert);
    char *sub = mpz_get_str(NULL, 10, m);
    mpz_set_str(convert, sub, 10);

    pow_mod(c, convert, e, n);
    free(sub);
    mpz_clear(convert);
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    uint8_t *block;
    uint64_t k = 0;
    mpz_t n2;
    mpz_t converti;
    mpz_t converto;
    mpz_init_set(n2, n);
    mpz_init(converti);
    mpz_init(converto);
    while (mpz_cmp_ui(n2, 0) > 0) {
        k++;
        mpz_fdiv_q_ui(n2, n2, 2);
    }
    k = ((k - 1) / 8);
    while (true) {
        block = (uint8_t *) calloc(k, sizeof(uint8_t));
        uint64_t read = fread(block, sizeof(uint8_t), k - 1, infile);
        for (uint64_t i = k - 1; i > 0; i--) {
            block[i] = block[i - 1];
        }
        block[0] = 255;
        mpz_import(converti, read + 1, 1, sizeof(block[0]), 1, 0, block);
        char *sub = mpz_get_str(NULL, 10, converti);
        mpz_set_str(converti, sub, 10);
        rsa_encrypt(converto, converti, e, n);
        gmp_fprintf(outfile, "%Zx\n", converto);
        free(block);
        free(sub);
        if (read != k - 1)
            break;
    }
    mpz_clears(n2, converti, converto, NULL);
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    mpz_t convert;
    mpz_init(convert);
    char *sub = mpz_get_str(NULL, 10, c);
    mpz_set_str(convert, sub, 10);

    pow_mod(m, convert, d, n);
    mpz_clear(convert);
    free(sub);
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    uint8_t *block;
    uint64_t k = 0;
    mpz_t n2;
    mpz_t converti;
    mpz_t converto;
    mpz_init_set(n2, n);
    mpz_init(converti);
    mpz_init(converto);
    while (mpz_cmp_ui(n2, 0) > 0) {
        k++;
        mpz_fdiv_q_ui(n2, n2, 2);
    }
    while (gmp_fscanf(infile, "%Zx\n", converti) != EOF) {
        rsa_decrypt(converto, converti, d, n);
        block = (uint8_t *) calloc(k, sizeof(uint8_t));
        size_t *cnt = calloc(1, sizeof(size_t));
        mpz_export(block, cnt, 1, sizeof(block[0]), 1, 0, converto);
        for (uint64_t i = 1; i < *cnt; i++) {
            fprintf(outfile, "%c", (char) block[i]);
        }
        free(block);
        free(cnt);
    }
    mpz_clears(n2, converti, converto, NULL);
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    mpz_t convert;
    mpz_init(convert);
    char *sub = mpz_get_str(NULL, 10, m);
    mpz_set_str(convert, sub, 10);

    pow_mod(s, m, d, n);
    free(sub);
    mpz_clear(convert);
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t convert2;
    mpz_t convert;
    mpz_t t;

    mpz_init(convert);
    mpz_init(convert2);
    mpz_init(t);

    char *sub1 = mpz_get_str(NULL, 10, s);
    char *sub2 = mpz_get_str(NULL, 10, m);
    mpz_set_str(convert, sub1, 10);
    mpz_set_str(convert2, sub2, 10);

    pow_mod(t, convert, e, n);

    bool ok = false;
    if (mpz_cmp(t, convert2) == 0)
        ok = true;

    free(sub1);
    free(sub2);
    mpz_clears(convert2, convert, t, NULL);
    return ok;
}
