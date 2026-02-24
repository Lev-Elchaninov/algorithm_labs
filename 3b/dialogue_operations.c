#include "dialogue.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

ErrorCode operation_input(Table** table){
    printf("Enter the key: ");
    unsigned key = 0;
    ErrorCode error = get_unsigned(&key);
    if (error != SUCCESS)
        return error;

    printf("Enter the information: ");
    unsigned info = 0;
    error = get_unsigned(&info);
    if (error != SUCCESS)
        return error;

    if (error != SUCCESS)
        return error;
    error = func_input(table, key, info);
    if (error != SUCCESS)
        return error;
    return SUCCESS;
}


ErrorCode operation_delete(Table* table){
    if (table == NULL || table->size == 0)
        return ERROR_EMPTY_TABLE;
    unsigned key = 0;
    printf("Enter the key: ");
    ErrorCode error = get_unsigned(&key);
    if (error != SUCCESS)
        return error;
    error = func_delete(table, key);
    if (error != SUCCESS)
        return error;
    return SUCCESS;
}



ErrorCode operation_search(const Table* table){
    if (table == NULL || table->keyspace == NULL || table->size == 0)
        return ERROR_EMPTY_TABLE;
    unsigned key = 0;
    ErrorCode error = ERROR_NOT_ALL;
    Data* data = malloc(sizeof(Data));
    data->array = NULL; data->size = 0; data->capacity = 5;
    while (error != SUCCESS){
        printf("Enter the key: ");
        error = get_unsigned(&key);
        if (error != SUCCESS){
            erase_data(data);
            return error;
        }
        Data_element* required_key = malloc(sizeof(Data_element));
        if (required_key == NULL){
            erase_data(data);
            return ERROR_MEMORY_ALLOCATION;
        }
        
        error = func_key_search(table, key, required_key);
        if (error == ERROR_EMPTY_TABLE){
            free(data);
            free(required_key);
            return error;
        }
        else if (error == ERROR_KEY_NOT_FOUND){
            free(required_key);
            errors_printf(error);
        }
        if (error == SUCCESS){
            error = entry(&data, *required_key);//не библиотечная функция
            free(required_key);
            error = not_all();//срашиваем у пользователя, все ли он ввел
            if (error == ERROR_EOF){
                erase_data(data);
                errors_printf(error);
                return error;
            }
        }
    }
    output_data(data);
    erase_data(data);
    return SUCCESS;    
}


ErrorCode operation_output_bin(const Table* table){
    if (table == NULL)
        return ERROR_EMPTY_TABLE;
    char* filename = readline("Enter the name of file: ");
    if (filename == NULL) return ERROR_EOF;
    ErrorCode error = func_output_bin(table, filename);
    if (error != SUCCESS){
        free(filename);
        return error;
    }
    free(filename);
    return SUCCESS;
}


ErrorCode operation_input_bin(Table** table){
    char* filename = readline("Enter the filename: ");
    if (filename == NULL)
        return ERROR_EOF;
    ErrorCode error = func_input_bin(table, filename);
    if (error != SUCCESS){
        free(filename);
        return error;
    }
    free(filename);
    return SUCCESS;
}