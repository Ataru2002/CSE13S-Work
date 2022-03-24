CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: integrate

integrate: integrate.o mathlib.o functions.o
	$(CC) -o integrate integrate.o mathlib.o functions.o

integrate.o: integrate.c mathlib.c functions.c
	$(CC) $(CFLAGS) -c integrate.c mathlib.c functions.c 

clean:
	rm -f integrate *.o

format:
	clang-format -i -style=file *.[ch]

