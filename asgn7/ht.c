#include "ht.h"
#include "salts.h"
#include "speck.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **slots;
};

struct HashTableIterator {
    HashTable *table;
    uint32_t slot;
};

HashTable *ht_create(uint32_t size) { //create a new hash table object
    HashTable *newh = (HashTable *) malloc(sizeof(HashTable)); //allocate memory for the hash table
    if (newh) {
        newh->salt[0] = SALT_HASHTABLE_LO; //set the salts for the hash table
        newh->salt[1] = SALT_HASHTABLE_HI;
        newh->size = size; //set the size from the parameter
        newh->slots = (Node **) calloc(size, sizeof(Node *)); //allocate memory for the Node array
    }
    return newh;
}

void ht_delete(HashTable **ht) {
    // delete the nodes in the node array
    for (uint32_t i = 0; i < ht_size(*ht); i++) {
        if ((*ht)->slots[i] != NULL) {
            node_delete(&((*ht)->slots[i]));
        }
    }
    free((*ht)->slots); //free the node array
    free(*ht); //free the hash table
    *ht = NULL;
}

uint32_t ht_size(HashTable *ht) { //get the hash table's size
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *word) { //lookup the desire word in the hash table
    uint32_t k = hash(ht->salt, word); //hash the word using the salt
    k = k % (ht->size); //modulo to make sure the element fits in the hash table's node array
    for (uint32_t i = 0; i < ht->size; i++) {
        k = (k + i) % (ht->size); //move through the node array
        if (ht->slots[k] == NULL)
            break;
        if (strcmp((ht->slots[k])->word, word) == 0) {
            return ht->slots[k];
        }
    }
    return NULL;
}

Node *ht_insert(HashTable *ht, char *word) {
    uint32_t key;
    if (ht_lookup(ht, word) != NULL) {
        Node *inc = ht_lookup(ht, word); //get the node
        inc->count += 1.0; //increase the node's count
        return inc;
    }
    key = hash(ht->salt, word);
    key %= ht->size;
    //this will add the node to the hash table
    for (uint32_t i = 0; i < ht->size; i++) {
        key = (key + i) % (ht->size);
        if (ht->slots[key] == NULL) {
            ht->slots[key] = node_create(word);
            return ht->slots[key];
        }
    }
    return NULL;
}

void ht_print(HashTable *ht) { //debugging function
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->slots[i] != NULL) {
            node_print(ht->slots[i]);
            printf("\n");
        }
    }
}

HashTableIterator *hti_create(HashTable *ht) { //make a hash table iterator
    HashTableIterator *sub = (HashTableIterator *) malloc(
        sizeof(HashTableIterator)); //allocate memory for the hash table iterator
    if (sub) {
        sub->table = ht; //set the table as the one in the parameter
        sub->slot = 0; //set the pointer to 0 as default value
    }
    return sub;
}

void hti_delete(HashTableIterator **hti) {
    if (hti) {
        free(*hti); //free the hash table iterator
        *hti = NULL;
    }
}

Node *ht_iter(HashTableIterator *hti) {
    while (hti->slot < ht_size(hti->table)
           && (hti->table)->slots[hti->slot] == NULL) //keep moving until a node is found
        hti->slot += 1;
    if (hti->slot == ht_size(hti->table)) //reached the end of the node array
        return NULL;
    hti->slot += 1;
    return (hti->table)->slots[hti->slot - 1]; //return the node
}
