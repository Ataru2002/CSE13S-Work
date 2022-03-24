#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *node_create(char *word) { //creates a new Node element
    Node *sub = malloc(sizeof(Node)); //allocate memory for the node container
    if (sub) {
        sub->word = strdup(word); //copy the word from the parameter to the node's word
        sub->count = 1; //set node's count to default value 1
    }
    return sub;
}

void node_delete(Node **n) {
    free((*n)->word); //free the word first
    free(*n); //free the node
    *n = NULL;
}

void node_print(Node *n) { //debug function
    if (n) {
        for (uint32_t i = 0; i < strlen(n->word); i++) {
            printf("%c", n->word[i]);
        }
        printf("\nCount: %u", n->count);
    }
}
