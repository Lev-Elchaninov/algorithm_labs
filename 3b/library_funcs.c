#include "library.h"
#include "expansion.h"//библиотека с расширением таблицы
#include "hidden_functions.h"//библиотека с скрытыми от пользователя функциями
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#define INIT_CAPACITY 5;


ErrorCode func_input(Table** table, unsigned key, unsigned info){
    if ((*table) == NULL)
        return ERROR_EMPTY_TABLE;
    ErrorCode error = SUCCESS;
    unsigned hash = hash_function(key, (*table)->capacity);

    if ((*table)->keyspace[hash] != NULL){
        error = individuality(*table, hash, key);//проверка ключа на индивидуальность
        if (error != SUCCESS)
            return error;
    }

    KeySpace* current = malloc(sizeof(KeySpace));
        if (current == NULL)
            return ERROR_MEMORY_ALLOCATION;
    current->key = key;
    current->information = info;


    input_current(table, hash, current);

    (*table)->size++;


    if (((double)(*table)->size/(*table)->capacity >= BORDER_RATIO)>= BORDER_RATIO){
        ErrorCode error = table_expansion(table);//расширение таблицы, если коэф > 0.7*/
        if (error != SUCCESS)
            return error;
    }
    return SUCCESS;
}



ErrorCode func_delete(Table* table, const unsigned key){
    if (table == NULL || table->keyspace == NULL)
        return ERROR_EMPTY_TABLE;
    unsigned key_hash = hash_function(key, table->capacity);
    KeySpace* ptr = table->keyspace[key_hash];
    KeySpace* prev = NULL;
    while (ptr != NULL && (ptr->key != key)){
        prev = ptr;
        ptr = ptr->next;
    }
    if (ptr != NULL && prev != NULL){//ключ не первый
        prev->next = ptr->next;
        table->size--;
        free(ptr);
    }
    else if (prev == NULL && ptr != NULL){//eлюч первый
        table->keyspace[key_hash] = table->keyspace[key_hash]->next;
        free(ptr);
        table->size--;
    }
    else if (ptr == NULL)
        return ERROR_KEY_NOT_FOUND;
    return SUCCESS;

}



ErrorCode func_key_search(const Table* table, const unsigned key, Data_element* required_element){
    if (table == NULL || table->keyspace == NULL)
        return ERROR_EMPTY_TABLE;
    unsigned key_hash = hash_function(key, table->capacity);
    KeySpace* ptr = table->keyspace[key_hash];
    while (ptr != NULL && ptr->key != key)
        ptr = ptr->next;
    if (ptr == NULL)
        return ERROR_KEY_NOT_FOUND;
    required_element->key = ptr->key;
    required_element->information = ptr->information;
    return SUCCESS;
}



ErrorCode func_output(const Table* table, FILE* file){
    if (table == NULL || file == NULL){
        return ERROR_EMPTY_TABLE;
    }
    for (unsigned i = 0; i < table->capacity; i++){
        KeySpace* ptr = table->keyspace[i];
        fprintf(file, "[%u] -> ", i);
        while (ptr != NULL){
            fprintf(file, "{key = %u | info = %u} -> ", ptr->key, ptr->information);
            ptr = ptr->next;
        }
        fprintf(file, "\n");
    }
    fprintf(file, "size = %u\n", table->size);
    return SUCCESS;
}



ErrorCode func_input_bin(Table** table, const char* filename){
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
        return ERROR_OPENING_FILE;
    erase_table(*table);
    (*table) = malloc(sizeof(Table));
    if (table == NULL)
        return ERROR_EMPTY_TABLE;

    fread(&(*table)->capacity, sizeof(unsigned), 1, file);
    if ((*table)->capacity == 0)
        return ERROR_EMPTY_TABLE;
    (*table)->size = 0;
    (*table)->keyspace = calloc((*table)->capacity, sizeof(KeySpace*));
    if ((*table)->keyspace == NULL)
        return ERROR_MEMORY_ALLOCATION;

    for (unsigned i = 0; i < (*table)->capacity; i++){
        unsigned size = 0;
        fread(&size, sizeof(unsigned), 1, file);
        for (unsigned j = 0; j < size; j++){
            KeySpace* current = malloc(sizeof(KeySpace));
                if (current == NULL)
                    return ERROR_MEMORY_ALLOCATION;
            fread(&current->key, sizeof(int), 1, file);
            fread(&current->information, sizeof(unsigned), 1, file);
            input_current(table, i, current);
            (*table)->size++;
        }   
    }
    fclose(file);
    return SUCCESS;
}


ErrorCode func_output_bin(const Table* table, const char* filename){
    if (table == NULL)
        return ERROR_EMPTY_TABLE;
    FILE* file = fopen(filename, "wb");
    if (file == NULL)
        return ERROR_OPENING_FILE;
    fwrite(&table->capacity, sizeof(unsigned), 1, file);//запись длины массива
    
    for (unsigned i = 0; i < table->capacity; i++){
        KeySpace* ptr = table->keyspace[i];
        unsigned size = size_count(table->keyspace[i]);
        fwrite(&size, sizeof(unsigned), 1, file);
        for (unsigned j = 0; j < size; j++){
            fwrite(&ptr->key, sizeof(unsigned), 1, file);
            fwrite(&ptr->information, sizeof(unsigned), 1, file);
            ptr = ptr->next;
        }
    }
    fclose(file);
    return SUCCESS;
}


void erase_table(Table* table){
    if (table == NULL)
        return;
    if (table->keyspace == NULL)
        return;
    for (unsigned i = 0; i < table->capacity; i++){
        KeySpace* ptr = table->keyspace[i];
        while (ptr != NULL){
            table->keyspace[i] = table->keyspace[i]->next;
            ptr->next = NULL;
            free(ptr);
            ptr = table->keyspace[i];
        }
    }
    free(table->keyspace);
    free(table);
}



ErrorCode init_table(Table** table){
    (*table) = malloc(sizeof(Table));
    if (*table == NULL) return ERROR_MEMORY_ALLOCATION;
    (*table)->capacity = INIT_CAPACITY;
    (*table)->size = 0;
    (*table)->keyspace = calloc((*table)->capacity, sizeof(KeySpace*));
    if ((*table)->keyspace == NULL){
        free(*table);
        return ERROR_MEMORY_ALLOCATION; 
    }
    return SUCCESS;
}
