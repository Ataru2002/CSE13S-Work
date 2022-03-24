CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: life

life: life.o universe.o
	$(CC) -o life -lncurses life.o universe.o

life.o: life.c universe.c
	$(CC) $(CFLAGS) -c life.c universe.c

clean:
	-rm -f life *.o

format:
	clang-format -i -style=file *.[ch]

