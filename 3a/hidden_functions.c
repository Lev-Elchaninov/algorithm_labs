#include "hidden_functions.h"
#include "queue.h"
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 1000

ErrorCode parent_check(const Table* table, char* parent){//надо
    if (table == NULL) return ERROR_EOF;
    if (table->keyspace == NULL && strcmp(parent, "null") != 0) return ERROR_PARENT_NOT_FOUND;
    if (strcmp(parent, "null") == 0) return SUCCESS;//это если таблица не пустая

    KeySpace* ptr = table->keyspace;
    while (ptr != NULL){
        if (strcmp(ptr->key, parent) == 0){
            return SUCCESS;
        }
        ptr = ptr->next;
    }
    return ERROR_PARENT_NOT_FOUND;
}



ErrorCode individuality(const Table* table, const char* key){//надо
    KeySpace* ptr = table->keyspace;
    while (ptr != NULL){
        if (ptr->key != NULL && strcmp(key, ptr->key) == 0){
            return ERROR_KEY_MATCH;
        }
        ptr = ptr->next;
    }
  
    return SUCCESS;
}




void free_keyspace(KeySpace* current){
    if (current != NULL) {
        free(current->key);
        free(current->par);
        free(current->information);
        free(current);
    }
}




ErrorCode add_key(char*** array, unsigned* count, unsigned* capacity, const char* key) {//добавление ключа в массив
    if (*count >= *capacity) {
        if (*capacity == 0)
            *capacity = 1;
        else{
            *capacity = *capacity * 2;
        }   
        char** ptr = realloc(*array, (*capacity) * sizeof(char*));
        if (ptr == NULL) {free_char_array(*array, *count); return ERROR_MEMORY_ALLOCATION;}
        (*array) = ptr;
    }
    (*array)[(*count)] = strdup(key);
    (*count)++;
    return SUCCESS;
}




ErrorCode init_queue(Queue* queue, unsigned initial_capacity){
    queue->data = malloc(initial_capacity * sizeof(char*));
    if (queue->data == NULL) return ERROR_MEMORY_ALLOCATION;
    queue->head = 0;
    queue->tail = 0;
    queue->capacity = initial_capacity;
    return SUCCESS;
}


ErrorCode enqueue(Queue* queue, const char* key){//добавляет элемент в очередь
    if (queue->head >= queue->capacity) {
        queue->capacity = queue->capacity * 2;
        char** ptr = realloc(queue->data, queue->capacity * sizeof(char*));
        if (ptr == NULL)
            {free_queue(queue); return ERROR_MEMORY_ALLOCATION;}
        else
            queue->data = ptr;
    }
    queue->data[queue->tail] = strdup(key);
    queue->tail++;
    return SUCCESS;
}



char* dequeue(Queue* queue){
    if (queue->head >= queue->tail)
        return NULL; //пустая очередь
    char* elem = queue->data[queue->head];
    queue->head++;
    return elem;
}


void free_queue(Queue* queue){
    for (unsigned i = queue->head; i < queue->tail; i++) {
        free(queue->data[i]);
    }
    free(queue->data);
}


void free_char_array(char** array, unsigned size){
    for (unsigned i = 0; i < size; i++)
        free(array[i]);
    free(array);
}





ErrorCode char_input_txt(char** string, FILE* file, Table* table, KeySpace* current){
    (*string) = malloc(MAX_SIZE * sizeof(char)); 
    if ((*string) == NULL) {
        free(current);
        return ERROR_MEMORY_ALLOCATION;
    }

    if (fgets((*string), MAX_SIZE, file) == NULL){
        free((*string)); 
        table->keyspace = current->next;
        free_keyspace (current); 
        return ERROR_EOF;
    } 
    (*string)[strlen((*string))-1] = '\0';
    char* ptr_txt = realloc((*string), (strlen((*string))+1)); 
    if (ptr_txt == NULL) {
        free((*string));
        return ERROR_MEMORY_ALLOCATION;
    }
        
    (*string) = ptr_txt;
    return SUCCESS;
}