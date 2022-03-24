CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: encode decode

encode: encode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o encode encode.o node.o pq.o code.o io.o stack.o huffman.o

encode.o: encode.c node.c pq.c code.c io.c stack.c huffman.c
	$(CC) $(CFLAGS) -c encode.c node.c pq.c code.c io.c stack.c huffman.c

decode: decode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o decode decode.o node.o pq.o code.o io.o stack.o huffman.o

decode.o: decode.c node.c pq.c code.c io.c stack.c huffman.c
	$(CC) $(CFLAGS) -c decode.c node.c pq.c code.c io.c stack.c huffman.c


clean:
	-rm -f encode *.o
	-rm -f decode *.o

format:
	clang-format -i -style=file *.[ch]



