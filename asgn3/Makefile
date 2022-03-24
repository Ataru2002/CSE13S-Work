CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: sorting

sorting: batcher.o quick.o heap.o insert.o stats.o sorting.o
	$(CC) -o sorting batcher.o quick.o heap.o insert.o stats.o sorting.o

sorting.o: batcher.c quick.c heap.c insert.c stats.c sorting.c
	$(CC) $(CFLAGS) -c batcher.c quick.c heap.c insert.c stats.c sorting.c

clean:
	-rm -f sorting *.o

format:
	clang-format -i -style=file *.[ch]
