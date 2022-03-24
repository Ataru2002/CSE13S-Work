#include "text.h"
#include "ht.h"
#include "bf.h"
#include "parser.h"
#include "node.h"
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>

#define WORD    "[a-zA-Z]+(['-][a-zA-Z]+)*" //the regex
#define HT_SIZE (1 << 19)
#define BF_SIZE (1 << 21)

struct Text {
    HashTable *ht;
    BloomFilter *bf;
    uint32_t word_count;
};

Text *text_create(FILE *infile, Text *noise) { //create the text object
    Text *t = (Text *) malloc(sizeof(Text)); //allocate memory for the Text object
    if (t) {
        regex_t re;
        char *words = NULL;
        if (regcomp(&re, WORD, REG_EXTENDED)) { //compile the regex
            free(t);
            fprintf(stderr, "Reg failed\n");
            return NULL;
        }
        t->ht = ht_create(HT_SIZE); //create the hash table
        if (t->ht == NULL) {
            free(t);
            fprintf(stderr, "Can't initialize the hash table");
            return NULL;
        }
        t->bf = bf_create(BF_SIZE); //create the bloom filter
        if (t->bf == NULL) {
            free(t);
            fprintf(stderr, "Can't initialize the bloom filter");
            return NULL;
        }
        t->word_count = 0; //set the word count to 0 by default
        while ((words = next_word(infile, &re)) != NULL) { //parsing the text
            for (uint32_t i = 0; i < strlen(words); i++)
                words[i] = tolower(words[i]); //convert the word to lowercase
            if ((noise != NULL && !text_contains(noise, words))
                || noise == NULL) { //filter through noise
                t->word_count++;
                ht_insert(t->ht, words);
                bf_insert(t->bf, words);
            }
        }
        regfree(&re);
    }
    return t;
}

void text_delete(Text **text) {
    if (text) {
        ht_delete(&((*text)->ht)); //delete the hash table
        bf_delete(&((*text)->bf)); //delete the bloom filter
        free(*text); //free the text
        *text = NULL;
    }
}

double distances(double d1, double d2, Metric metric) { //calculate the distance base on the metric
    if (metric == EUCLIDEAN)
        return (d1 - d2) * (d1 - d2);
    else if (metric == MANHATTAN)
        return fabs(d1 - d2);
    return d1 * d2;
}

double text_dist(Text *text1, Text *text2, Metric metric) { //calculate the distance
    HashTableIterator *hti1 = hti_create(text1->ht); //create the hash table iterators
    HashTableIterator *hti2 = hti_create(text2->ht);
    Node *sub = NULL;
    double f1 = 0.0;
    double f2 = 0.0;
    double total = 0.0;
    while ((sub = ht_iter(hti1)) != NULL) { //iterate through the hash table from text1
        f1 = text_frequency(text1, sub->word); //get the frequency normalized
        f2 = text_frequency(text2, sub->word);
        total += distances(f1, f2, metric);
    }
    while ((sub = ht_iter(hti2)) != NULL) { //iterate through the hash table from text2
        if (text_contains(text1, sub->word) && text_contains(text2, sub->word))
            continue; //check for duplicates to avoid overcounting
        f1 = text_frequency(text1, sub->word);
        f2 = text_frequency(text2, sub->word);
        total += distances(f1, f2, metric);
    }
    if (metric == EUCLIDEAN)
        total = (double) sqrt(total); //square root the answer for the euclidean distance
    if (metric == COSINE)
        total = 1 - total; //1 - the total for the cosine distance
    hti_delete(&hti1);
    hti_delete(&hti2);
    return total;
}

double text_frequency(Text *text, char *word) {
    if (bf_probe(text->bf, word)) { //check if the word is in the bloom filter first
        Node *t = NULL;
        if ((t = ht_lookup(text->ht, word)) == NULL) //check if the word is in the hash table
            return 0.0;
        else
            return (double) t->count / text->word_count; //calculate the frequency normalized
    }
    return 0.0;
}

bool text_contains(Text *text, char *word) {
    return text_frequency(text, word)
           != 0; //check if the frequency is 0 which indicate the text doesn't contain the word
}

void text_print(Text *text) { //debugging function
    ht_print(text->ht);
    //bf_print(text->bf);
    printf("%u\n", text->word_count);
}
