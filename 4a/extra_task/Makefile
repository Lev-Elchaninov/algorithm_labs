CC = gcc
CFLAGS = -c -Wall -g -Wextra -O2
LDFLAGS = -lreadline -lm

all: task


application.o: task.o main.o insert_to_bin.o
		@$(LD) -r task.o main.o -o application.o  insert_to_bin.o

main.o: main.c
		@$(CC) $(CFLAGS) main.c

task.o: task.c
		@$(CC) $(CFLAGS) task.c

insert_to_bin.o: insert_to_bin.c
		@$(CC) $(CFLAGS) insert_to_bin.c

task: library.a application.o
		@$(CC) library.a application.o -o task $(LDFLAGS)

		@rm -rf *.o 
clean:
		rm -rf *.o task

.PHONY: all clean
