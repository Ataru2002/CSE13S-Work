#include "node.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *news = malloc(sizeof(Node)); //allocate memory for the node object
    if (news) {
        news->left = NULL; //set the node's left child to default value NULL
        news->right = NULL; //set the node's right child to default value NULL
        news->symbol = symbol; //set the node's symbol to the one in the parameter
        news->frequency = frequency; //set the node's frequency to the one in the parameter
    }
    return news;
}

void node_delete(Node **n) {
    if (n != NULL) {
        free(*n); //free the node object
        *n = NULL;
    }
}

Node *node_join(Node *left, Node *right) {
    Node *parent = malloc(sizeof(Node));
    if (parent) {
        parent->left = left;
        parent->right = right;
        parent->symbol = (uint8_t) '$';
        parent->frequency = (left->frequency) + (right->frequency);
    }
    return parent;
}

void node_print(Node *n) {

    if (n)
        printf("Node is good\n");
    if (n == NULL) {
        printf("Node is not created\n");
        return;
    }
    printf("Symbol: %c\n", n->symbol);
    printf("Frequency: %" PRIu64 "\n", n->frequency);

    if (n->left)
        printf("Left node is connected\n");
    else if (n->left == NULL)
        printf("Left node is not connected\n");

    if (n->right)
        printf("Right node is connected\n");
    else if (n->right == NULL)
        printf("Right node is not connected\n");

    //printf("%" PRIu64 " ", n->frequency);
}
