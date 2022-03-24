#include "pq.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Pair Pair;

struct Pair { //this data structure stores both the author and the distance
    char *author;
    double dist;
};

struct PriorityQueue {
    uint32_t front;
    uint32_t tail;
    uint32_t capacity;
    Pair **items;
};

PriorityQueue *pq_create(uint32_t capacity) { //create a new priority queue
    PriorityQueue *newp
        = (PriorityQueue *) malloc(sizeof(PriorityQueue)); //allocate memory for the priority queue
    if (newp) {
        newp->front = newp->tail = 0; //set both the tail and front pointer to 0
        newp->capacity = capacity; //set the capacity to the capacity in the parameter
        newp->items
            = (Pair **) malloc(capacity * sizeof(Pair *)); //allocate memory for the Pair array
    }
    return newp;
}

void pq_delete(PriorityQueue **q) {
    if (q) {
        //free the content in the Pair array
        for (uint32_t i = 0; i < (*q)->capacity; i++) {
            if ((*q)->items[i]) {
                free(((*q)->items[i])->author);
                free((*q)->items[i]);
            }
        }
        free((*q)->items); //free the Pair array
        free(*q); //free the priority queue
        *q = NULL;
    }
}

bool pq_empty(PriorityQueue *q) { //check if the priority queue is empty
    return q->tail == 0;
}

bool pq_full(PriorityQueue *q) { //check if the priority queue is full
    return q->tail == q->capacity;
}

uint32_t pq_size(PriorityQueue *q) { //get the size of the priority queue
    return q->tail;
}

uint32_t parent(uint32_t i) { //get the parent node from the current node
    return ((i - 1) / 2);
}

uint32_t left_child(uint32_t i) { //get the left child node from the current node
    return i * 2 + 1;
}

uint32_t right_child(uint32_t i) { //get the right child node from the current node
    return i * 2 + 2;
}

void swap(Pair **p1, Pair **p2) { //swap the 2 pair objects
    Pair *sub = *p1;
    *p1 = *p2;
    *p2 = sub;
}

void fix_heap(PriorityQueue *q, uint32_t start) { //fixes the Pair array to become a min heap
    uint32_t l = left_child(start);
    uint32_t r = right_child(start);
    uint32_t small = start;
    if (l < q->tail && (q->items[l])->dist < (q->items[start])->dist)
        small = l;
    if (r < q->tail && (q->items[r])->dist < (q->items[small])->dist)
        small = r;
    if (small != start) {
        swap(&(q->items[small]), &(q->items[start]));
        fix_heap(q, small);
    }
}

bool enqueue(
    PriorityQueue *q, char *author, double dist) { //push an element into the priority queue
    if (pq_full(q))
        return false;
    uint32_t i = q->tail;

    Pair *insert = malloc(sizeof(Pair)); //make a Pair object
    if (insert) {
        insert->author = strdup(author);
        insert->dist = dist;
    } else
        return false;

    q->items[i] = insert;
    while (
        i != 0
        && (q->items[parent(i)])->dist
               > (q->items[i])
                     ->dist) { //while the inserted Pair is not in the right position, keep moving up a floor
        swap(&(q->items[parent(i)]), &(q->items[i]));
        i = parent(i);
    }
    q->tail += 1;
    return true;
}

bool dequeue(PriorityQueue *q, char **author, double *dist) { //pop an element out of the queue
    if (pq_empty(q))
        return false;
    *author = ((q->items[q->front])->author); //return the author
    *dist = ((q->items[q->front])->dist); //return the distance

    swap(&(q->items[q->front]), &(q->items[q->tail - 1])); //swap the front Pair to the tail Pair

    q->tail -= 1;
    fix_heap(q, 0); //fix the array from 0 to tail pointer
    return true;
}

void pq_print(PriorityQueue *q) { //debugging function
    for (uint32_t i = q->front; i < q->tail; i++) {
        printf("%s %f\n", (q->items[i])->author, (q->items[i])->dist);
    }
}
