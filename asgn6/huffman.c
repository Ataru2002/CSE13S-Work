#include "node.h"
#include "code.h"
#include "pq.h"
#include "stack.h"
#include "io.h"
#include "defines.h"
#include <stdint.h>
#include <stdio.h>

Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *pq = pq_create(256);
    Node *n1 = NULL;
    Node *n2 = NULL;
    Node *n3 = NULL;
    Node *sub = NULL;
    for (int i = 0; i < ALPHABET; i++) {
        if (hist[i] > 0) {
            sub = node_create(i,
                hist[i]); //insert the node to the priority queue if the histogram value of the letter
            enqueue(pq, sub);
        }
    }
    while (pq_size(pq) >= 2) {
        dequeue(pq, &n1);
        dequeue(pq, &n2);
        n3 = node_join(n1,
            n2); //dequeue 2 nodes from the priority queue then enqueue a new node with 2 nodes joined
        enqueue(pq, n3);
    }
    Node *ans = NULL;
    dequeue(pq, &ans); //get the last node of the priority queue which is the huffman tree
    pq_delete(&pq);
    return ans;
}

void travel(Node *curn, Code *curc, Code table[static ALPHABET]) {
    if (curn == NULL)
        return;
    if (curn->left == NULL && curn->right == NULL) {
        //get the path of the current leaf and insert them into the code table
        for (uint32_t i = 0; i < curc->top; i++) {
            if (code_get_bit(curc, i))
                code_push_bit(&(table[curn->symbol]), 1);
            else
                code_push_bit(&(table[curn->symbol]), 0);
        }

        return;
    }
    uint8_t popped;

    code_push_bit(curc, 0);
    travel(curn->left, curc, table);
    code_pop_bit(curc, &popped);

    code_push_bit(curc, 1);
    travel(curn->right, curc, table);
    code_pop_bit(curc, &popped);
    //traversing the huffman tree
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    if (root) {
        Code c = code_init();
        travel(root, &c, table);
    }
}

void dump_tree(int outfile, Node *root) {
    if (root) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        if (root->left == NULL || root->right == NULL) {
            uint8_t print[2] = { 'L', root->symbol }; //content to push for a leaf
            write_bytes(outfile, print, 2);
        } else {
            uint8_t print[1] = { 'I' }; //content to push for the interior nodes
            write_bytes(outfile, print, 1);
        }
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *s = stack_create(nbytes);
    for (uint16_t i = 0; i < nbytes; i++) {
        if (tree[i] == 'L') {
            stack_push(s, node_create(tree[i + 1], 0)); //push leaf node into the stack
            i++;
        } else if (tree[i] == 'I') {
            Node *n1 = NULL;
            Node *n2 = NULL;
            stack_pop(s, &n1);
            stack_pop(s, &n2);
            stack_push(s,
                node_join(n2,
                    n1)); //pop 2 nodes then push the node that combines to 2 popped nodes together
        }
    }
    Node *ans = NULL;
    stack_pop(s, &ans); //get the popped node which is the rebuilt huffman tree
    stack_delete(&s);
    return ans;
}

void delete_tree(Node **root) {
    if (*root != NULL) {
        if (((*root)->left) != NULL) //delete the tree in a post-order traversal
            delete_tree(&((*root)->left));
        if (((*root)->right) != NULL)
            delete_tree(&((*root)->right));
        node_delete(root);
    }
}
