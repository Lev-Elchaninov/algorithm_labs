CC = gcc
CFLAGS = -c -Wall -Wextra -g

all: example

example: inoutput.o input_check.o main.o task.o
		$(CC) inoutput.o input_check.o main.o task.o -o example

inoutput.o: inoutput.c
		$(CC) $(CFLAGS) inoutput.c

input_check.o: input_check.c
		$(CC) $(CFLAGS) input_check.c

main.o: main.c
		$(CC) $(CFLAGS) main.c

task.o: task.c
		$(CC) $(CFLAGS) task.c

clean:
		rm -rf *.o example

.PHONY: all clean
