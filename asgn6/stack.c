#include "stack.h"
#include "node.h"
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

Stack *stack_create(uint32_t capacity) {
    Stack *news = (Stack *) malloc(sizeof(Stack)); //allocate memory for the Stack object
    if (news) {
        news->top = 0; //set top to default value of 0
        news->capacity = capacity; //set capcity to value in the parameter
        news->items
            = (Node **) malloc(capacity * sizeof(Node *)); //allocate memory for the node array
    }
    return news;
}

void stack_delete(Stack **s) {
    if (s) {
        free((*s)->items); //free the node array
        free(*s); //free the stack
        *s = NULL;
    }
}

bool stack_empty(Stack *s) {
    return (s->top == 0);
}

bool stack_full(Stack *s) {
    return (s->top == s->capacity);
}

uint32_t stack_size(Stack *s) {
    return s->top;
}

bool stack_push(Stack *s, Node *n) {
    if (stack_full(s))
        return false;
    s->items[s->top] = n; //set the top node to the push node
    s->top += 1; //increment the top pointer
    return true;
}

bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s))
        return false;
    s->top -= 1; //decrease the top pointer
    *n = s->items[s->top]; //return popped node
    return true;
}

void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->capacity; i++)
        node_print(s->items[i]);
}
