CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g -pg -Ofast
LFLAGS = -lm


all: identify

identify: identify.o node.o pq.o ht.o speck.o bv.o bf.o text.o parser.o
	$(CC) -o identify identify.o node.o pq.o ht.o speck.o bv.o bf.o text.o parser.o $(LFLAGS)

identify.o: identify.c node.c pq.c ht.c speck.c bv.c bf.c text.c parser.c
	$(CC) $(CFLAGS) -c identify.c node.c pq.c ht.c speck.c bv.c bf.c text.c parser.c 


clean:
	-rm -f identify *.o

scan-build: clean
	scan-build --use-cc=$(CC) make

format:
	clang-format -i -style=file *.[ch]




