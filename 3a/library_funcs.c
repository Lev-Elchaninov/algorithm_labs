#include "library.h"
#include "hidden_functions.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <readline/readline.h>

//1/ функция вставки + инит + очищение таблицы
ErrorCode input_element(Table* table, KeySpace* current){//надо
    if (strcmp(current->key, "null" ) == 0)
        return ERROR_FORBIDDEN_KEY;
    if (table == NULL || current == NULL) return ERROR_NULL_POINTER;
    if (table->keyspace == NULL){
        table->keyspace = current;
        current->next = NULL;
    }
    else if(table->keyspace != NULL){
        current->next = table->keyspace;
        table->keyspace = current;
    }
    
    return SUCCESS;
}




ErrorCode init_table(Table** table){
    (*table) = calloc(1, sizeof(Table)); 
    if (*table == NULL)
        return ERROR_MEMORY_ALLOCATION;
    return SUCCESS;
}


void erase_table(Table* table){
    if (table == NULL) return;
    KeySpace* ptr = table->keyspace;
    while (table->keyspace != NULL){
        ptr = table->keyspace;
        free(table->keyspace->key); free(table->keyspace->par); free(table->keyspace->information);
        table->keyspace = table->keyspace->next;
        free(ptr);
    }
}





//2/ Функция удаления
ErrorCode delete(Table* table, char* key) {
    if (strcmp(key, "null") == 0)
        return ERROR_FORBIDDEN_KEY;
    KeySpace* ptr = NULL;
    ErrorCode error = key_search(table, key, &ptr);
    if (error != ERROR_KEY_MATCH){
        key = NULL;
        return ERROR_KEY_NOT_FOUND;
    }

    Queue queue;
    error = init_queue(&queue, 10);
    if (error != SUCCESS) return error;

    error = enqueue(&queue, key);
    if (error != SUCCESS) return error;

    char** keys_to_delete = NULL;
    unsigned del_current = 0, del_capacity = 0;
    error = add_key(&keys_to_delete, &del_current, &del_capacity, key);//добавляет ключи в массив ключей для удаления
    if (error != SUCCESS) {free_queue(&queue); return error;}

    // Собираю ключи в массивчик
    while (queue.head < queue.tail) {
        char* current_key = dequeue(&queue);
        KeySpace* node = table->keyspace;

        while (node != NULL) {
            if (node->par != NULL && strcmp(node->par, current_key) == 0) {//если нашли потомка нашего ключа        
                error = enqueue(&queue, node->key);
                if (error != SUCCESS) return error;
                error = add_key(&keys_to_delete, &del_current, &del_capacity, node->key);
                if (error != SUCCESS) {free_queue(&queue); return error;}
            }
            node = node->next;
        }
        free(current_key);
    }
    if (error != SUCCESS)
        return error;
    //сама удаление элементов после того как я обработал все ключи
    KeySpace* prev = NULL;
    KeySpace* current = table->keyspace;
    while (current != NULL) {
        KeySpace* next = current->next;
        int should_delete = 0;
        for (unsigned i = 0; i < del_current; i++) {
            if (strcmp(current->key, keys_to_delete[i]) == 0) {
                should_delete = 1;
                i = del_current;
            }
        }
        if (should_delete != 0) {
            if (prev != NULL) prev->next = next;
            else table->keyspace = next;

            free_keyspace(current);
        } 
        else prev = current;
        
        current = next;
    }
    free_queue(&queue);
    free_char_array(keys_to_delete, del_current);
    return SUCCESS;
}



//3. функция поиска
ErrorCode key_search(const Table* table, char* key, KeySpace** required_key){
    (*required_key) = table->keyspace;
    if (table->keyspace == NULL) {free(key); return ERROR_NULL_POINTER;} 
    while (strcmp((*required_key)->key, key) != 0){
        (*required_key) = (*required_key)->next;
        if ((*required_key) == NULL){
            free(key);
            return ERROR_KEY_NOT_FOUND;
        }
    }
    return ERROR_KEY_MATCH;
}


//4. функция вывода
ErrorCode output(const Table* table, FILE* file){
    if (table == NULL || table->keyspace == NULL) return ERROR_EMPTY_TABLE;
    KeySpace* ptr = table->keyspace;
    printf("\n");
    while (ptr != NULL){
        fprintf(file, "[key = %s, par = %s] -> ", ptr->key, ptr->par);
        if (ptr->information != NULL)
            fprintf(file, "{Info = %u}", *ptr->information);
        else if (ptr->information == NULL)
            fprintf(file, "{Info = NULL}");
        ptr = ptr->next;
        fprintf(file, "\n");
    }
    return SUCCESS;
}



//5. Импорт из текстового
ErrorCode file_input(Table* table, char* filename){
    ErrorCode error = SUCCESS;
    int correct_fields = 0;
    FILE* file = fopen(filename, "r"); 
    if (file == NULL)
        return ERROR_OPENING_FILE;

    erase_table(table);
    table->keyspace = NULL;
    while (error != ERROR_EOF){
        KeySpace* current = malloc(sizeof(KeySpace)); 
        if (current == NULL) {
            fclose(file);
            return ERROR_MEMORY_ALLOCATION;
        }

        current->information = NULL; current->key = NULL; current->par = NULL;
        current->next = table->keyspace;
        table->keyspace = current;


        char* key_txt = NULL;//начало занесения ключа
        error = char_input_txt(&key_txt, file, table, current);
        if (error == ERROR_MEMORY_ALLOCATION){
            fclose(file);
            return error;
        }
        if (error == ERROR_EOF){
            fclose(file);
            return SUCCESS;
        }
        if (key_txt != NULL){
            if (strcmp(key_txt, "null") == 0){
                fclose(file);
                free(key_txt);
                table->keyspace = table->keyspace->next;
                free(current);
                return ERROR_FORBIDDEN_KEY;
            }
        }

        error = individuality(table, key_txt); 
        if (error == ERROR_KEY_MATCH){
            free(key_txt); 
            table->keyspace = current->next;
            free_keyspace(current); 
            return error; 
        }
        
        current->key = key_txt;//конец занесения ключа
        
    



        char* par_txt = NULL;//начало занесения родителя
        error = char_input_txt(&par_txt, file, table, current);
        if (error == ERROR_EMPTY_TABLE){
            free(key_txt);
            fclose(file);
            return error;
        }
        if (error == ERROR_EOF){//дошли до конца файла
            fclose(file);   
            return SUCCESS;
        }

        error = parent_check(table, par_txt); 
        if(error == ERROR_PARENT_NOT_FOUND || strcmp(par_txt, key_txt) == 0) {
            free(par_txt); 
            table->keyspace = current->next;
            fclose(file); 
            free_keyspace(current);
            return ERROR_PARENT_NOT_FOUND;
        }
        current->par = par_txt;//конец занесения родителя
        
        
        int test = 0;//начало занесения значения
        correct_fields = fscanf(file, "%u\n", &test); 
        if (test < 0 || correct_fields == 0) {
            table->keyspace = current->next;
            free_keyspace(current); 
            return ERROR_NOT_UNSIGNED;
        }
        current->information = malloc(sizeof(unsigned)); 
        if (current->information == NULL)
            return ERROR_MEMORY_ALLOCATION;
        *current->information = (unsigned)test;//конец занесения значения
        
    }
    fclose(file);
    return SUCCESS;
}


//6. (Индив) Удаление элема + его дети - null
ErrorCode delete_elem_and_kids_null(Table* table, char* key){
   
    KeySpace* ptr = table->keyspace;
    KeySpace* prev = table->keyspace;

    KeySpace* test = NULL;
    ErrorCode error = key_search(table, key, &test);
    if (error != ERROR_KEY_MATCH){
        key = NULL;
        return ERROR_KEY_NOT_FOUND;
    }
    else{
        ptr = table->keyspace;
        prev = table->keyspace;
    }

    while (ptr != NULL){
        if (strcmp(ptr->par, key) == 0){//ищем его потомков
            free(ptr->par);
            ptr->par = strdup("null");
            prev = ptr;
            ptr = ptr->next;
        }
        else if (strcmp(ptr->par, key) != 0 && strcmp(ptr->key, key) != 0){
            prev = ptr;
            ptr = ptr->next;
        }
        else if (strcmp(ptr->key, key) == 0){//нашли нужный нам ключ
            if (ptr == table->keyspace)
                table->keyspace = table->keyspace->next;
            prev->next = ptr->next;
            free_keyspace(ptr);
            ptr = NULL;
        }
    }
    return SUCCESS;
}


//7. (Индив) Поиск в диапазоне 
ErrorCode individual_search(const Table* table, Table** resoult, const char* border_1, const char* border_2){
    KeySpace* ptr = table->keyspace;
    (*resoult) = malloc(sizeof(Table)); 
    if (resoult == NULL) {return ERROR_MEMORY_ALLOCATION;}
    (*resoult)->keyspace = NULL;
    
    while (ptr != NULL){
        if (strcmp(border_1, ptr->par) <= 0 && strcmp(ptr->par, border_2) <= 0 && strcmp(ptr->par, "null") != 0){
                KeySpace* current = calloc(1, sizeof(KeySpace));
                if (current == NULL)
                    {free((*resoult)); return ERROR_MEMORY_ALLOCATION;}
                current->key = strdup(ptr->key);
                current->par = strdup(ptr->par);
                current->information = malloc(sizeof(unsigned));
                if (current->information == NULL)
                    {free(current->par); free(current->key); free(current); free((*resoult));}
                memcpy(current->information, ptr->information, sizeof(unsigned));

            ErrorCode error = input_element(*resoult, current);
            if (error != SUCCESS) return error;
          
        }
        ptr = ptr->next;
    }

    return SUCCESS;
}


