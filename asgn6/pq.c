#include "pq.h"
#include "node.h"
#include <stdlib.h>
#include <stdio.h>

struct PriorityQueue {
    uint32_t front;
    uint32_t tail;
    uint32_t capacity;
    Node **items;
};

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *newp = (PriorityQueue *) malloc(
        sizeof(PriorityQueue)); //allocate memory for the priority queue object
    if (newp) {
        newp->front = newp->tail = 0; //set the front and tail to default value 0
        newp->capacity = capacity; //set the capacity to the capacity in the parameter
        newp->items
            = (Node **) malloc(capacity * sizeof(Node *)); //allocate memory for the node array
    }
    return newp;
}

void pq_delete(PriorityQueue **q) {
    if (q) {
        free((*q)->items); //free the node array
        free(*q); //free the priority queue object
        *q = NULL;
    }
}

bool pq_empty(PriorityQueue *q) {
    return (q->front == q->tail);
}

bool pq_full(PriorityQueue *q) {
    return (q->tail == q->capacity);
}

uint32_t pq_size(PriorityQueue *q) {
    return q->tail;
}

uint32_t parent(uint32_t i) {
    return ((i - 1) / 2); //return the index of the parent node
}

uint32_t left_child(uint32_t i) {
    return i * 2 + 1; //return the index of the left child node
}

uint32_t right_child(uint32_t i) {
    return i * 2 + 2; //return the index of the right child node
}

void swap(Node **n1, Node **n2) {
    Node *sub = *n1;
    *n1 = *n2;
    *n2 = sub;
}

//this function fixes the node array to be a heap when called from 0 to the queue's tail value
void fix_heap(PriorityQueue *q, uint32_t start) {
    uint32_t l = left_child(start);
    uint32_t r = right_child(start);
    uint32_t small = start;
    if (l < q->tail && (q->items[l])->frequency < (q->items[start])->frequency)
        small = l;
    if (r < q->tail && (q->items[r])->frequency < (q->items[small])->frequency)
        small = r;
    if (small != start) {
        swap(&(q->items[small]), &(q->items[start]));
        fix_heap(q, small);
    }
}

bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q))
        return false;
    uint32_t i = q->tail;

    q->items[i] = n; //set the top node to the node in parameter

    //this while loop places the newly pushed node in the correct position
    while (i != 0 && (q->items[parent(i)])->frequency > (q->items[i])->frequency) {
        swap(&(q->items[parent(i)]), &(q->items[i]));
        i = parent(i);
    }

    q->tail += 1; //increment the tail value
    return true;
}

bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q))
        return false;
    *n = q->items[q->front]; //return the popped node
    q->items[q->front]
        = q->items[q->tail - 1]; //set the node from the front the node pointed by the tail pointer
    q->tail -= 1; //decrease the tail value
    fix_heap(q, 0); //fix the heap
    return true;
}

void pq_print(PriorityQueue *q) {
    for (uint32_t i = q->front; i < q->tail; i++)
        node_print(q->items[i]);
    printf("\n");
}
