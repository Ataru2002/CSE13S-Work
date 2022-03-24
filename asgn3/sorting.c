#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "stats.h"

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage() {
    fprintf(stderr, "Synopsis\n"
                    "	Generate a random array and sort the array using different sorting "
                    "algorithm while also provide the statistics of each sorting algorithm in "
                    "terms of number of moves and comparisons	 "
                    "Usage\n"
                    "	type -[a, e, b, i, q] -p [an unsigned int] -n [an unsigned int] -r [an "
                    "unsigned int]\n"
                    "Options\n"
                    "	-a	Employs all sorting algorithms\n"
                    "	-h	Employs the Heap Sort algorithm\n"
                    "	-b	Employs the Batcher Sort algorithm\n"
                    "	-i	Employs the Insertion Sort algorihm\n"
                    "	-r seed		Set random seed (Default: 13371453)\n"
                    "	-n size		Set the array size (Default: 100)\n"
                    "	-p elements	Print out the number of elements in the array (Default: "
                    "100), if elements is larger than size then print out the whole array\n"
                    "	-H: Display the program usage and synopsis\n");
}

int main(int argc, char **argv) {
    Stats test;
    char *ptr;
    int opt;
    uint32_t seed = 13371453;
    uint32_t size = 100;
    uint32_t elements = 100;
    Set coms = empty_set();
    while ((opt = getopt(argc, argv, "ahbiqr:n:p:H")) != -1) {
        switch (opt) {
        case 'a':
            coms = insert_set(0, coms);
            coms = insert_set(1, coms);
            coms = insert_set(2, coms);
            coms = insert_set(3, coms);
            break;
        case 'h': coms = insert_set(0, coms); break;
        case 'b': coms = insert_set(1, coms); break;
        case 'i': coms = insert_set(2, coms); break;
        case 'q': coms = insert_set(3, coms); break;
        case 'r': seed = (uint32_t) strtoul(optarg, &ptr, 10); break;
        case 'n': size = (uint32_t) strtoul(optarg, &ptr, 10); break;
        case 'p': elements = (uint32_t) strtoul(optarg, &ptr, 10); break;
        case 'H':
            usage();
            return 0;
            break;
        }
    }
    uint32_t *testarr = NULL;
    testarr = malloc(size * sizeof(uint32_t));
    uint32_t cnt = 0;
    if (testarr == NULL)
        return EXIT_FAILURE;
    while (cnt < 4) {
        if ((coms & 1) == 1) {
            srandom(seed);
            for (uint32_t i = 0; i < size; i++) {
                testarr[i] = (uint32_t)(random() & (uint32_t)((1 << 30) - 1));
            }
            if (cnt == 0) {
                heap_sort(&test, testarr, size);
                printf("Heap sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares\n",
                    size, test.moves, test.compares);
            } else if (cnt == 1) {
                batcher_sort(&test, testarr, size);
                printf("Batcher sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64
                       " compares\n",
                    size, test.moves, test.compares);
            } else if (cnt == 2) {
                insertion_sort(&test, testarr, size);
                printf("Insertion sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64
                       " compares\n",
                    size, test.moves, test.compares);
            } else {
                quick_sort(&test, testarr, size);
                printf("Quick sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64
                       " compares\n",
                    size, test.moves, test.compares);
            }
            uint32_t print = (size < elements) ? size : elements;
            for (uint32_t i = 0; i < print; i++) {
                printf("%13" PRIu32 " ", testarr[i]);
                if ((i + 1) % 5 == 0 && i != print - 1)
                    printf("\n");
            }
            printf("\n");
        }
        cnt++;
        coms /= 2;
    }
    free(testarr);
    return 0;
}
