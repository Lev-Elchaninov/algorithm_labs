CC = gcc
CFLAGS = -c -Wall -g -Wextra
LDFLAGS = -lreadline 

all: app

library.a: library_funcs.o table_expansion.o hidden_functions.o
		@$(LD) -r library_funcs.o table_expansion.o hidden_functions.o -o library.a

library_funcs.o: library_funcs.c
		@$(CC) $(CFLAGS) library_funcs.c

table_expansion.o: table_expansion.c
		@$(CC) $(CFLAGS) table_expansion.c

hidden_functions.o: hidden_functions.c
		@$(CC) $(CFLAGS) hidden_functions.c


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
		@$(CC) library.a application.o -o app $(LDFLAGS)

		@rm -rf *.o 
clean:
		rm -rf *.o app *.a

.PHONY: all clean
