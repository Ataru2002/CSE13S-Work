#include "node.h"
#include "stack.h"
#include "pq.h"
#include "code.h"
#include "io.h"
#include "huffman.h"
#include "defines.h"
#include "header.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void usage() {
    fprintf(stderr,
        "Synopsis\n"
        "   Performs an encoding using the Huffman tree\n"
        "Usage\n"
        "   -i [file name] -o [file name] -v\n"
        "Options\n"
        "   -i infile       Specifies the input file to encode (Default: stdin)\n"
        "   -o outfile      Specifies the output file for the encoding process  (Default: stdout)\n"
        "   -v              Output compression statistics\n"
        "   -h              Display the program usage and synopsis\n");
}

int main(int argc, char **argv) {
    int opt;
    bool stats = false;
    int input = STDIN_FILENO; //set the input file to stdin
    int output = STDOUT_FILENO; //set the output file to stdout
    FILE *tempf = tmpfile(); //temporary file for stdin
    int temp = fileno(tempf);
    uint64_t hist[ALPHABET]; //make the historgram
    Code table[ALPHABET]; //make the code table
    uint64_t cnt = 0;
    for (uint64_t i = 0; i < ALPHABET; i++) { //set the histogram and code table to default values
        hist[i] = 0;
        table[i] = code_init();
    }
    hist[0] = 1;
    hist[255] = 1;
    struct stat fst;
    struct stat fst2;
    while ((opt = getopt(argc, argv, "i:o:vh")) != -1) { //get the commands
        switch (opt) {
        case 'i':
            close(input);
            input = open(optarg, O_RDONLY);
            if (input == -1) {
                printf("Can't Open Input file\n");
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            close(output);
            output = open(optarg, O_WRONLY);
            if (output == -1) {
                printf("Can't Open Output file\n");
                return EXIT_FAILURE;
            }
            break;
        case 'v': stats = true; break;
        case 'h':
            usage();
            return 0;
            break;
        }
    }
    uint8_t inarr[BLOCK];
    while (true) { //loop to read the bytes from the input file to the temporary file
        uint64_t actr = read_bytes(input, inarr, 1);
        if (actr == 0)
            break;
        write_bytes(temp, inarr, 1);
        hist[inarr[0]]++;
        inarr[0] = 0;
    }
    for (int i = 0; i < ALPHABET; i++)
        cnt += (hist[i] > 0);
    input = temp; //set the input file to the temporary file
    lseek(input, 0, SEEK_SET); //reset the input pointer
    Node *tree = build_tree(hist);
    build_codes(tree, table); //build the code table
    fstat(input, &fst);
    fchmod(input, fst.st_mode); //set the permission to both input and output file
    fchmod(output, fst.st_mode);
    Header t;
    t.magic = MAGIC;
    t.permissions = fst.st_mode;
    t.tree_size = 3 * cnt - 1;
    t.file_size = fst.st_size;
    uint8_t *outs = (uint8_t *) &t; //translate the header to bytes
    write_bytes(
        output, outs, sizeof(Header) / sizeof(uint8_t)); //write the bytes to the output file
    dump_tree(output, tree); //dump the tree to the output
    while (true) { //read the bytes in BLOCK from the input file (now the temporary file)
        uint64_t actr = read_bytes(input, inarr, BLOCK);
        if (actr <= 0)
            break;
        for (uint64_t i = 0; i < actr; i++)
            write_code(output, &table[inarr[i]]); //write the code to the output
        for (uint64_t i = 0; i < BLOCK; i++)
            inarr[i] = 0;
    }
    flush_codes(output); //flush out the rest of the codes
    if (stats) { //print the statistics
        fstat(output, &fst2);
        fprintf(stderr, "Uncompressed size: %lu bytes\n ", t.file_size);
        fprintf(stderr, "Compressed size: %lu bytes\n", fst2.st_size);
        fprintf(stderr, "Save: %f%%\n",
            (float) 100 * (1 - ((float) fst2.st_size / (float) t.file_size)));
    }
    close(input);
    close(output);
    fclose(tempf);
    delete_tree(&tree);
    return 0;
}
