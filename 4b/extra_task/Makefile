CC = gcc
CFLAGS = -c -Wall -g -Wextra
LDFLAGS = -lreadline -lm

all: app

task: main.o
		@$(CC) main.o -o task $(LDFLAGS)



app: ../dialogue_programm/library.a main.o
		@$(CC) ../dialogue_programm/library.a main.o -o task $(LDFLAGS)



		@rm -rf *.o 
clean:
		rm -rf *.o bin/app *.a

.PHONY: all clean
