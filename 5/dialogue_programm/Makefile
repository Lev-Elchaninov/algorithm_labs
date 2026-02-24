CC = gcc
CFLAGS = -c -Wall -g -Wextra
LDFLAGS = -lreadline -lm

all: app

library.a: library_funcs.o hidden_funcs.o stack.o
		@$(LD) -r library_funcs.o hidden_funcs.o stack.o -o library.a

library_funcs.o: library_funcs.c
		@$(CC) $(CFLAGS) library_funcs.c

hidden_funcs.o: hidden_funcs.c
		@$(CC) $(CFLAGS) hidden_funcs.c

stack.o: stack.c
		@$(CC) $(CFLAGS) stack.c



application.o: dialogue.o inoutput.o main.o dialogue_operations.o
		@$(LD) -r dialogue.o inoutput.o main.o dialogue_operations.o -o application.o 

dialogue.o: dialogue.c
		@$(CC) $(CFLAGS) dialogue.c

inoutput.o: inoutput.c
		@$(CC) $(CFLAGS) inoutput.c
	
main.o: main.c
		@$(CC) $(CFLAGS) main.c

dialogue_operations.o: dialogue_operations.c
		@$(CC) $(CFLAGS) dialogue_operations.c

app: library.a application.o
		@$(CC) application.o library.a -o app $(LDFLAGS)

		@rm -rf *.o 
clean:
		rm -rf *.o app *.a

.PHONY: all clean
