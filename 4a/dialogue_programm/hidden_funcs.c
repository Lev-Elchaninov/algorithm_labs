#include "hidden_functions.h"
#include "stack.h"
#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 1000//для fgets

ErrorCode node_init(Node** current, const size_t key, const char* info){//создание узла дерева
    (*current) = malloc(sizeof(Node));
    if ((*current) == NULL)
        return ERROR_MEMORY_ALLOCATION;
    (*current)->info_fieldes.info = strdup(info);
    (*current)->info_fieldes.key = key;
    (*current)->left = NULL;
    (*current)->right = NULL;
    (*current)->parent = NULL;
    return SUCCESS;
}



ErrorCode char_input_txt(FILE* file, char** string){
    (*string) = malloc(MAX_SIZE * sizeof(char)); 
    if (*string == NULL){
        return ERROR_MEMORY_ALLOCATION;
    }

    if (fgets(*string, MAX_SIZE, file) == NULL){
        free(*string);  
        return ERROR_EOF;
    }
    if (*string == NULL)
        return ERROR_EOF;
    
    size_t len = strlen(*string);
    if (len > 0 && (*string)[len-1] == '\n')
        (*string)[len-1] = '\0';
        
    char* ptr_txt = realloc((*string), (strlen((*string))+1)); 
    if (ptr_txt == NULL) {
        free((*string));
        return ERROR_MEMORY_ALLOCATION;
    }
    (*string) = ptr_txt;
    return SUCCESS;
}
