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

Node *mroot;
uint64_t cnt = 0;

Node *traverse(int outfile, Node *root, uint8_t dir) {
    if (root->right == NULL && root->left == NULL) {
        uint8_t out;
        out = root->symbol;
        write_bytes(outfile, &out, 1); //write the bytes out from the code in the input
        cnt++;
        root = mroot;
    }
    if (dir == 0)
        return root->left;
    if (dir == 1)
        return root->right;
    return NULL;
}

void usage() {
    fprintf(stderr,
        "Synopsis\n"
        "   Performs a decoding for the encoded message from the encoder\n"
        "Usage\n"
        "   -i [file name] -o [file name] -v\n"
        "Options\n"
        "   -i infile       Specifies the input file to decode (Default: stdin)\n"
        "   -o outfile      Specifies the output file for the decoding process  (Default: stdout)\n"
        "   -v              Output compression statistics\n"
        "   -h              Display the program usage and synopsis\n");
}

int main(int argc, char **argv) {
    int opt;
    bool stats = false;
    int input = STDIN_FILENO;
    int output = STDOUT_FILENO;
    while ((opt = getopt(argc, argv, "i:o:vh")) != -1) { //get command
        switch (opt) {
        case 'i':
            close(input);
            input = open(optarg, O_RDONLY);
            if (input == -1) {
                printf("Can't open input file\n");
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            close(output);
            output = open(optarg, O_WRONLY);
            if (output == -1) {
                printf("Can't open output file\n");
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
    Header ins;
    uint8_t *reader = malloc((sizeof(Header) / sizeof(uint8_t)) * sizeof(uint8_t));
    read_bytes(input, reader, 16); //read the header bytes
    ins = *((Header *) (reader));
    if (ins.magic != 3203382189) { //check if the magic number match
        printf("Magic number doesn't match\n");
        return EXIT_FAILURE;
    }
    fchmod(output, ins.permissions);
    uint8_t dtree[ins.tree_size];
    read_bytes(input, dtree, ins.tree_size);
    Node *tree = rebuild_tree(
        ins.tree_size, dtree); //rebuild the tree from the dump tree in the input file
    Node *curnode = tree;
    mroot = tree;
    while (cnt < ins.file_size) { //read the bit and traverse the tree accordingly
        uint8_t curbit;
        bool actr = read_bit(input, &curbit);
        if (!actr)
            break;
        curnode = traverse(output, curnode, curbit);
    }
    if (stats) { //output the statistics
        struct stat fst;
        struct stat fst2;
        fstat(input, &fst);
        fstat(output, &fst2);
        fprintf(stderr, "Compressed size: %lu bytes\n", fst.st_size);
        fprintf(stderr, "Decompressed size: %lu bytes\n", fst2.st_size);
        fprintf(stderr, "Save: %f%%\n",
            (float) 100 * (1 - ((float) fst.st_size / (float) fst2.st_size)));
    }
    close(input);
    close(output);
    delete_tree(&tree);
    free(reader);
}
