CC = gcc
CFLAGS = -c -Wall -g

all: example

example: oldmatrix.o newmatrix.o main.o 
		$(CC) oldmatrix.o newmatrix.o main.o -o example

oldmatrix.o: oldmatrix.c
		$(CC) $(CFLAGS) oldmatrix.c

newmatrix.o: newmatrix.c
		$(CC) $(CFLAGS) newmatrix.c

main.o: main.c
		$(CC) $(CFLAGS) main.c

clean:
		rm -rf *.o example

.PHONY: all clean

