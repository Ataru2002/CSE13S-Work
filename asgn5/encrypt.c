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
#include <assert.h>

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
    fprintf(stderr, "Synopsis\n"
                    "   Performs a RSA Encryption using the keys from keygen"
                    "Usage\n"
                    "   -i [file name] -o [file name] -n [file name] -v\n"
                    "Options\n"
                    "   -i infile	Specifies the input file to encrypt (Default: stdin)\n"
                    "   -o outfile	Specifies the output file to output the encrypted message "
                    "(Default: stdout)\n"
                    "   -n pbfile       Specifies the public key file (Default: rsa.pub)\n"
                    "   -v              Enables verbose output\n"
                    "   -h              Display the program usage and synopsis\n");
}

int main(int argc, char **argv) {
    int opt;
    FILE *public;
    FILE *input = stdin;
    FILE *output = stdout;
    bool verbose = false;
public
    = fopen("rsa.pub", "r+");
    if (public == NULL) {
        fprintf(stdout, "Can't open public key file\n");
        return EXIT_FAILURE;
    }
    while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
        switch (opt) {
        case 'i':
            input = fopen(optarg, "r+");
            if (input == NULL) {
                fprintf(stdout, "Can't open input file\n");
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            output = fopen(optarg, "r+");
            if (output == NULL) {
                fprintf(stdout, "Can't open output file\n");
                return EXIT_FAILURE;
            }
            break;
        case 'n':
            fclose(public);
        public
            = fopen(optarg, "w+");
            if (public == NULL) {
                fprintf(stdout, "Can't open public key file\n");
                return EXIT_FAILURE;
            }
            break;
        case 'v': verbose = true; break;
        case 'h':
            usage();
            return 0;
            break;
        }
    }
    char readname[256] = { 0 };
    mpz_t n, e, s, name;
    mpz_init(n);
    mpz_init(e);
    mpz_init(s);
    mpz_init(name);
    rsa_read_pub(n, e, s, readname, public);
    if (verbose) {
        fprintf(stdout, "user = %s\n", readname);
        gmp_fprintf(stdout, "s (%lu bits): %Zd\n", cntbits(s), s);
        gmp_fprintf(stdout, "n (%lu bits): %Zd\n", cntbits(n), n);
        gmp_fprintf(stdout, "e (%lu bits): %Zd\n", cntbits(e), e);
    }
    mpz_set_str(name, readname, 62);
    if (!rsa_verify(name, s, e, n)) {
        fprintf(stdout, "Can't verify\n");
        return EXIT_FAILURE;
    }
    rsa_encrypt_file(input, output, n, e);
    fclose(public);
    fclose(input);
    fclose(output);
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(s);
    mpz_clear(name);
    return 0;
}
