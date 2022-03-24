CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)

all: keygen encrypt decrypt

keygen: keygen.o randstate.o numtheory.o rsa.o
	$(CC) -o keygen keygen.o randstate.o numtheory.o rsa.o $(LFLAGS)

keygen.o: keygen.c randstate.c numtheory.o rsa.o
	$(CC) $(CFLAGS) -c keygen.c randstate.c numtheory.c rsa.c

encrypt: encrypt.o randstate.o numtheory.o rsa.o
	$(CC) -o encrypt encrypt.o randstate.o numtheory.o rsa.o $(LFLAGS)

encrypt.o: encrypt.c randstate.c numtheory.o rsa.o
	$(CC) $(CFLAGS) -c encrypt.c randstate.c numtheory.c rsa.c

decrypt: decrypt.o randstate.o numtheory.o rsa.o
	$(CC) -o decrypt decrypt.o randstate.o numtheory.o rsa.o $(LFLAGS)

decrypt.o: decrypt.c randstate.c numtheory.o rsa.o
	$(CC) $(CFLAGS) -c decrypt.c randstate.c numtheory.c rsa.c



clean:
	-rm -f keygen *.o
	-rm -f encrypt *.o
	-rm -f decrypt *.o

format:
	clang-format -i -style=file *.[ch]


