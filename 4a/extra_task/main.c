#include <stdlib.h>
#include <stdio.h>
#include "../dialogue_programm/library.h"
#include <readline/readline.h>

ErrorCode task(Bin_tree*, char*);
void generate_and_write_size_t_array(char*, size_t, size_t);


int main(){
    Bin_tree* tree = NULL;
    ErrorCode error = init_tree(&tree);
    if (error != SUCCESS)
        return error;
    char* input_filename = readline("Enter file to write numbers: ");
    if (input_filename == NULL){
        erase_tree(tree);
        printf("\nerror: EOF\n\n");
        return ERROR_EOF;
    }
    size_t max_count = 200;
    size_t max_value = 100;
    generate_and_write_size_t_array(input_filename, max_count, max_value);
    free(input_filename);
    char* new_input = readline("Enter new input file (from what I will read): ");
    if (new_input == NULL){
        erase_tree(tree);
        printf("\nerror: EOF\n\n");
        return ERROR_EOF;
    }
    error = task(tree, new_input);
    if (error != SUCCESS){
        erase_tree(tree);
        free(new_input);
        if (error == ERROR_EOF)
            printf("\nerror: EOF\n\n");
        else if (error == ERROR_OPENING_FILE)
            perror("Error opening file: ");
        return error;
    }
    erase_tree(tree);
    free(new_input);
    return 0;
}