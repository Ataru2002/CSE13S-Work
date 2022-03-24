#include "code.h"
#include <inttypes.h>
#include <stdio.h>

Code code_init(void) {
    Code newc;
    newc.top = 0; //set the code pointer to default value 0
    for (uint32_t i = 0; i < MAX_CODE_SIZE; i++)
        newc.bits[i] = 0; //set all the value in the code to 0
    return newc;
}

uint32_t code_size(Code *c) {
    return c->top;
}

bool code_empty(Code *c) {
    return c->top == 0;
}

bool code_full(Code *c) {
    return c->top == 256;
}

bool code_set_bit(Code *c, uint32_t i) {
    if (i > c->top)
        return false;
    uint32_t pos = i / 8; //find the position of which byte to change
    uint32_t val = i % 8; //find the position of which bit to change in the byte
    c->bits[pos] |= (1 << val); //set that bit to 1
    return true;
}

bool code_clr_bit(Code *c, uint32_t i) {
    if (i > c->top)
        return false;
    uint32_t pos = i / 8; //find the position of which byte to change
    uint32_t val = i % 8; //find the position of which bit to change in the byte
    c->bits[pos] &= ~(1 << val); //set that bit to 0
    return true;
}

bool code_get_bit(Code *c, uint32_t i) {
    if (i > c->top)
        return false;
    uint32_t pos = i / 8;
    uint32_t val = i % 8;
    return ((c->bits[pos] & (1 << val)) > 0); //get the bit's state
}

bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c))
        return false;
    if (bit == 1)
        code_set_bit(c, c->top); //set the pushed bit to 1
    else
        code_clr_bit(c, c->top); //set the pushed bit to 0
    c->top++; //increase the value of the code pointer
    return true;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c))
        return false;
    *bit = code_get_bit(c, c->top - 1); //return the popped bit
    code_clr_bit(c, c->top - 1); //clear the current bit position
    c->top--; //decrease the value of the code pointer
    return true;
}

void code_print(Code *c) {
    for (uint32_t i = 0; i < MAX_CODE_SIZE; i++)
        printf("%u ", c->bits[i]);
    printf("\n");
}
