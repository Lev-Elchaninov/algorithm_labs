CC = gcc
CFLAGS = -c -Wall -g -Wextra -O2
LDFLAGS = -lreadline -lm

all: test


application.o: test.o
		@$(LD) -r test.o -o application.o 
	
test.o: test.c
		@$(CC) $(CFLAGS) test.c

test: ../dialogue_programm/library.a application.o
		@$(CC) ../dialogue_programm/library.a application.o -o test $(LDFLAGS)

		@rm -rf *.o 
clean:
		rm -rf *.o test

.PHONY: all clean
