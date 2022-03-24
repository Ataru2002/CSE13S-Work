#include "functions.h"
#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//calculate the integrated value using the Simpson's 1/3 rule
double integrate(double (*f)(double), double a, double b, uint32_t n) {
    double term1 = 0.0; //term 1 of the equation in the sum
    double term2 = 0.0; //term 2 of the equation in the sum
    double term3 = 0.0; //term 3 of the equation in the sum
    double h = (b - a) / n;
    //for loop for adding up the values in the sum
    for (double j = 1.0; j <= n / 2; j++) {
        term1 += (*f)(a + ((2 * j - 2) * h));
        term2 += (*f)(a + ((2 * j - 1) * h));
        term3 += (*f)(a + (2 * j * h));
    }
    term2 *= 4.0;
    return (h / 3) * (term1 + term2 + term3);
}

void usage() {
    fprintf(stderr, "Synopsis\n"
                    "	Print the integrate value of a function of choice from a specified low to "
                    "high range\n"
                    "Usage\n"
                    "	type -[from a to j] -p [a real number] -q [a real number] -n [an integer]\n"
                    "Options\n"
                    "	-a	integrate Sqrt(1 - x^4)\n"
                    "	-b	integrate 1 / log(x)\n"
                    "	-c	integrate e^(-x^2)\n"
                    "	-d	integrate sin(x^2)\n"
                    "	-e	integrate cos(x^2)\n"
                    "	-f	integrate log(log(x))\n"
                    "	-g	integrate sin(x) / x\n"
                    "	-h	integrate e^(-x) / x\n"
                    "	-i	integrate e^(e^x)\n"
                    "	-j	integrate Sqrt(sin^2(x) + cos^2(x))\n"
                    "	-n partitions	set the number of partitions to use in the composite "
                    "Simpson's rule. The default value is 100\n"
                    "	-p low		set the low end of the interval to integrate to low, this "
                    "value is not set by default\n"
                    "	-q high		set the high end of the interval to integrate to high, "
                    "this value is not set by default\n"
                    "	-H: Display the program usage and synopsis\n");
}

int main(int argc, char **argv) {
    int opt;
    int segments = 100;
    double low;
    double high;
    char com;
    char *ptr;
    double (*pass)(double);
    // setup the commands
    while ((opt = getopt(argc, argv, "abcdefghijHp:q:n:")) != -1) {
        switch (opt) {
        case 'a':
            pass = &a;
            com = 'a';
            break;
        case 'b':
            pass = &b;
            com = 'b';
            break;
        case 'c':
            pass = &c;
            com = 'c';
            break;
        case 'd':
            pass = &d;
            com = 'd';
            break;
        case 'e':
            pass = &e;
            com = 'e';
            break;
        case 'f':
            pass = &f;
            com = 'f';
            break;
        case 'g':
            pass = &g;
            com = 'g';
            break;
        case 'h':
            pass = &h;
            com = 'h';
            break;
        case 'i':
            pass = &i;
            com = 'i';
            break;
        case 'j':
            pass = &j;
            com = 'j';
            break;
        case 'n': segments = (uint32_t) strtoul(optarg, &ptr, 10); break;
        case 'p': low = (double) strtod(optarg, &ptr); break;
        case 'q': high = (double) strtod(optarg, &ptr); break;
        case 'H': com = 'H'; break;
        }
    }
    //printing the title of the output
    switch (com) {
    case 'a': printf("Sqrt(1 - x^4), %f, %f, %i\n", low, high, segments); break;
    case 'b': printf("1 / log(x), %f, %f, %i\n", low, high, segments); break;
    case 'c': printf("e^-x^2, %f, %f, %i\n", low, high, segments); break;
    case 'd': printf("sin(x^2), %f, %f, %i\n", low, high, segments); break;
    case 'e': printf("cos(x^2), %f, %f, %i\n", low, high, segments); break;
    case 'f': printf("log(log(x)), %f, %f, %i\n", low, high, segments); break;
    case 'g': printf("sin(x) / x, %f, %f, %i\n", low, high, segments); break;
    case 'h': printf("e^-x / x), %f, %f, %i\n", low, high, segments); break;
    case 'i': printf("e^e^x, %f, %f, %i\n", low, high, segments); break;
    case 'j': printf("sqrt(sin^2(x) + cos^2(x)), %f, %f, %i\n", low, high, segments); break;
    case 'H':
        usage();
        return 0;
        break;
    default: return EXIT_FAILURE;
    }

    for (int i = 2; i <= segments; i += 2) {
        printf("%i, %0.9f\n", i, integrate(pass, low, high, i));
    }
    return 0;
}
