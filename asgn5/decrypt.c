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
                    "   Performs a decryption on an encrypted message to get the original message"
                    "Usage\n"
                    "   -i [file name] -o [file name] -n [file name] -v\n"
                    "Options\n"
                    "   -i infile       Specifies the input file to decrypt (Default: stdin)\n"
                    "   -o outfile      Specifies the output file to output the decrypted message "
                    "(Default: stdout)\n"
                    "   -n pbfile       Specifies the public key file (Default: rsa.priv)\n"
                    "   -v              Enables verbose output\n"
                    "   -h              Display the program usage and synopsis\n");
}

int main(int argc, char **argv) {
    int opt;
    FILE *private;
    FILE *input = stdin;
    FILE *output = stdout;
    bool verbose = false;
private
    = fopen("rsa.priv", "r+");
    if (private == NULL) {
        fprintf(stdout, "Can't open private key file\n");
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
            fclose(private);
        private
            = fopen(optarg, "r+");
            if (private == NULL) {
                fprintf(stdout, "Can't open private key file\n");
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

    mpz_t n, d;
    mpz_init(n);
    mpz_init(d);
    rsa_read_priv(n, d, private);
    if (verbose) {
        gmp_fprintf(stdout, "n (%lu bits): %Zd\n", cntbits(n), n);
        gmp_fprintf(stdout, "d (%lu bits): %Zd\n", cntbits(d), d);
    }

    rsa_decrypt_file(input, output, n, d);
    fclose(private);
    fclose(input);
    fclose(output);
    mpz_clear(n);
    mpz_clear(d);

    return 0;
}
