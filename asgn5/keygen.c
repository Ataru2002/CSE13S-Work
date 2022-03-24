/*
Author: Nhan Nguyen
Class: CSE13s
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>
#include <inttypes.h>
#include <time.h>
#include <sys/stat.h>

#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

uint64_t cntbits(mpz_t in) {
    mpz_t convert;
    mpz_init_set(convert, in);

    uint64_t cnt = 0;
    while (mpz_cmp_ui(convert, 0) > 0) {
        cnt++;
        mpz_fdiv_q_ui(convert, convert, 2);
    }
    mpz_clear(convert);
    return cnt;
}

void usage() {
    fprintf(stderr,
        "Synopsis\n"
        "	Generates the keys necessary to do a RSA Encryption "
        "Usage\n"
        "	-b [an unsigned int] -i [an unsigned int] -n [file name] -d [file name] -s[an "
        "unsigned int] -v\n"
        "Options\n"
        "	-b bits		Set the minimum number of bits for modulus n\n"
        "	-i iter		Set the number of iterations for the Miller-Rabin (Default: 50)\n"
        "	-n pbfile	Specifies the public key file (Default: rsa.pub)\n"
        "	-d pvfile	Specifies the private key file (Default: rsa.priv)\n"
        "	-s seed 	Specifies the seed for srandom() and the random state for GMP "
        "(Default: time(NULL))\n"
        "	-v		Enables verbose output\n"
        "	-h 		Display the program usage and synopsis\n");
}

int main(int argc, char **argv) {
    int opt;
    char *ptr;
    uint64_t bits = 0;
    uint64_t iter = 50;
    uint64_t seed = time(NULL);
    bool verbose = false;
    FILE *public;
    FILE *private;
public
    = fopen("rsa.pub", "r+");
private
    = fopen("rsa.priv", "r+");

    if (public == NULL) {
        printf("Bad public file\n");
        return EXIT_FAILURE;
    }
    if (private == NULL) {
        printf("Bad private file\n");
        return EXIT_FAILURE;
    }
    int fd = fileno(private);
    if (fd == -1) {
        printf("Can't get file description\n");
        return EXIT_FAILURE;
    }
    int perm = fchmod(fd, 0600);
    if (perm == -1) {
        printf("Can't change file mode\n");
        return EXIT_FAILURE;
    }
    while ((opt = getopt(argc, argv, "b:i:n:d:s:vh")) != -1) {
        switch (opt) {
        case 'b': bits = (uint64_t) strtoul(optarg, &ptr, 10); break;
        case 'i': iter = (uint64_t) strtoul(optarg, &ptr, 10); break;
        case 'n':
            fclose(public);
        public
            = fopen(optarg, "r+");
            if (public == NULL) {
                printf("Bad public file\n");
                return EXIT_FAILURE;
            }
            break;
        case 'd':
            fclose(private);
        private
            = fopen(optarg, "r+");
            if (private == NULL) {
                printf("Bad private file\n");
                return EXIT_FAILURE;
            }
            int fd = fileno(private);
            if (fd == -1) {
                printf("Can't get file description\n");
                return EXIT_FAILURE;
            }
            int perm = fchmod(fd, 0600);
            if (perm == -1) {
                printf("Can't change file mode\n");
                return EXIT_FAILURE;
            }
            break;
        case 's': seed = (uint64_t) strtoul(optarg, &ptr, 10); break;
        case 'v': verbose = true; break;
        case 'h':
            usage();
            return 0;
            break;
        }
    }
    randstate_init(seed);
    mpz_t p, q, n, e, d, s, name;

    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(e);
    mpz_init(d);
    mpz_init(s);
    mpz_init(name);

    rsa_make_pub(p, q, n, e, bits, iter);
    rsa_make_priv(d, e, p, q);
    char *user = getenv("USER");
    mpz_set_str(name, user, 62);
    rsa_sign(s, name, d, n);
    rsa_write_pub(n, e, s, user, public);
    rsa_write_priv(n, d, private);
    if (verbose) {
        fprintf(stdout, "user : ");
        mpz_out_str(stdout, 62, name);
        fprintf(stdout, "\n");
        gmp_fprintf(stdout, "s (%lu bits): %Zd\n", cntbits(s), s);
        gmp_fprintf(stdout, "p (%lu bits): %Zd\n", cntbits(p), p);
        gmp_fprintf(stdout, "q (%lu bits): %Zd\n", cntbits(q), q);
        gmp_fprintf(stdout, "n (%lu bits): %Zd\n", cntbits(n), n);
        gmp_fprintf(stdout, "e (%lu bits): %Zd\n", cntbits(e), e);
        gmp_fprintf(stdout, "d (%lu bits): %Zd\n", cntbits(d), d);
    }
    fclose(public);
    fclose(private);
    randstate_clear();
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(s);
    mpz_clear(name);
    return 0;
}
