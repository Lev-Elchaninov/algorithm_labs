#include "talk.h"

#include <stdio.h>
#include <stdlib.h>

ErrorCode operation_input(Table* table){
    ErrorCode error = SUCCESS;
    KeySpace* current = malloc(sizeof(KeySpace)); 
    if (current == NULL) {
        return ERROR_EOF;
    } 
    error = element_entry(table, current);
    if (error != SUCCESS && error != ERROR_KEY_MATCH) {
        errors_printf(error); 
        free(current); 
        return error;
    }            
    if (error != ERROR_KEY_MATCH){
        error = input_element(table, current);
        if (error == ERROR_FORBIDDEN_KEY){
            errors_printf(error);
            free(current->key);
            free(current->par);
            free(current->information);
            free(current);
        }
        else if (error != SUCCESS) 
            return error;
    }
    return SUCCESS;
}



ErrorCode operation_delete(Table* table){
    ErrorCode error = empty_table_check(table);
    if (error == ERROR_EMPTY_TABLE){
        errors_printf(error); 
        return error;
    }
    char* key_delete = NULL;
    error = key_input(&key_delete); 
    if (error != SUCCESS){
        errors_printf(error); 
        return error;
    }
    error = delete(table, key_delete); 
    if (error != ERROR_MEMORY_ALLOCATION)
        errors_printf(error);
    else
        return error;
    free(key_delete);
    return SUCCESS;
}



ErrorCode operation_search(const Table* table){
    ErrorCode error = empty_table_check(table);
    if (error == ERROR_EMPTY_TABLE){errors_printf(error); return error;}
    char* key = NULL; 
    error = ERROR_KEY_NOT_FOUND; 
    KeySpace* required_key = NULL; 
    Data* data = NULL;
    while (error != SUCCESS){
        while (error != ERROR_KEY_MATCH){
            error = key_input(&key); 
            if (error != SUCCESS){
                errors_printf(error); erase_data(data); 
                return error;
            }
            error = key_search(table, key, &required_key);
            if (error == ERROR_KEY_NOT_FOUND) errors_printf(error);
        }
        error = entry(&data, required_key);
        if (error != SUCCESS){erase_data(data); return error;}

        free(key);
        error = not_all();//спрашиваю у пользователя, все ли ключи он ввел
        if (error == ERROR_EOF){
            erase_data(data); 
            errors_printf(error); 
            return error;
        }
    }
    out_data(data); 
    erase_data(data);
    return SUCCESS;
}


ErrorCode operation_file_input(Table* table){
    ErrorCode error = SUCCESS;
    char* filename = input_filename();
    if (filename == NULL){ 
        errors_printf(ERROR_EOF); 
        return ERROR_EOF;
    }
    error = file_input(table, filename); 
    if (error != ERROR_EOF)
        errors_printf(error);
    else{
        free(filename);
        return error;
    }
    free(filename);
    return SUCCESS;
}


ErrorCode operation_delete_elem_and_kids_null(Table* table){
    ErrorCode error = empty_table_check(table);
    if (error == ERROR_EMPTY_TABLE) {
        errors_printf(error); 
        return error;
    }
    char* key_for_delete = NULL; 
    error = key_input(&key_for_delete); 
    if (error != SUCCESS){
        errors_printf(error); 
        return error;
    }
    
    error = delete_elem_and_kids_null(table, key_for_delete); 
    if (error != SUCCESS) 
        errors_printf(error);
 
    if (error != ERROR_KEY_NOT_FOUND)
        free(key_for_delete);
    return SUCCESS;
}


ErrorCode operation_search_in_range(const Table* table){
    ErrorCode error = empty_table_check(table);
    if (error == ERROR_EMPTY_TABLE){errors_printf(error); return error;}

    char* border_1 = NULL; char* border_2 = NULL;
    error = borders_input(&border_1, &border_2);
    if (error == ERROR_EOF) {errors_printf(error); return error;}

    Table* resoult = NULL; 
    error = individual_search(table, &resoult, border_1, border_2); 
    if (error != SUCCESS) {free(border_1); free(border_2); return error;} 

    error = output(resoult, stdout);
    erase_table(resoult); 
    free(resoult); 
    free(border_1); 
    free(border_2);
    return SUCCESS;
}