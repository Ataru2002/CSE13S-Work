#include "code.h"
#include "io.h"
#include "defines.h"
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
uint8_t buffer[BLOCK]; //buffer for the read_bit
uint8_t bufferc[BLOCK]; //buffer for the write_code
uint64_t curposc = 0; //pointer for the write_code
uint64_t curpos = 0; //pointer for read_bit

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int assign = nbytes;
    int cur = 0;
    int readb = 0;
    while (true) { //loop to make sure the read() function read the bytes assigned
        readb = read(infile, buf, assign);
        if (readb == 0) //exit when there're no more bytes to read
            break;
        cur += readb;
        assign -= cur;
        if (assign == 0) //the assigned bytes are read
            break;
    }
    //bytes_read += cur;
    return cur;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int assign = nbytes;
    int cur = 0;
    int writeb = 0;
    while (true) { //loop to make sure the write() function write the bytes assigned
        writeb = write(outfile, buf, assign);
        if (writeb == 0) //exit when there're no more bytes to write
            break;
        cur += writeb;
        assign -= cur;
        if (assign == 0) //the assigned bytes are write
            break;
    }
    //bytes_written = cur;
    return cur;
}

bool read_bit(int infile, uint8_t *bit) {
    if (curpos % (BLOCK * 8) == 0) {
        for (uint64_t i = 0; i < BLOCK; i++) //clear the bytes in the buffer before reading
            buffer[i] = 0;
        bytes_read += read_bytes(infile, buffer, BLOCK); //read the bytes and put them in buffer
    }
    if (curpos == (bytes_read * 8)) //all the bits are read
        return false;
    uint64_t pos = (curpos % (BLOCK * 8)) / 8; //find the byte position in the buffer
    uint64_t val = (curpos % (BLOCK * 8)) % 8; //find the bit position in the byte
    *bit = (uint8_t)((buffer[pos] & (1 << val)) > 0); //get the state of that bit
    curpos++;
    return true;
}

void write_code(int outfile, Code *c) {
    uint64_t pos;
    uint64_t val;
    for (uint64_t i = 0; i < c->top; i++) {
        pos = (curposc % (BLOCK * 8)) / 8;
        val = (curposc % (BLOCK * 8)) % 8;
        if (code_get_bit(c, i))
            bufferc[pos] |= (1 << val); //if bit is 1 then set the bit in the buffer to 1
        else
            bufferc[pos] &= (255 ^ (1 << val)); //if bit is 0 then set the bit in the buffer to 0
        curposc++;
        if (curposc % (BLOCK * 8) == 0) {
            bytes_written += write_bytes(
                outfile, bufferc, BLOCK); //write all the current bytes from the buffer out
            for (uint64_t j = 0; j < BLOCK; j++) //clear the buffer
                bufferc[j] = 0;
        }
    }
}

void flush_codes(int outfile) {
    uint64_t pos
        = ((curposc - 1) % (BLOCK * 8)) / 8; //find the last position where there are bytes to flush
    bytes_written += write_bytes(outfile, bufferc, pos + 1); //flush the bytes
}
