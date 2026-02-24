#include "expansion.h"


#include <stdio.h>
#include <stdlib.h>


const unsigned simple_numbers[] = {5, 11, 23, 47, 97, 199, 401, 809, 1619, 3251, 6521, 13049, 26111,
    52249, 104549, 209123, 418349, 836917, 1673809, 3347621, 6695263, 13390517, 26781037, 
    53562089, 107124127, 214248359, 428496719, 856993439, 1713986857, 3427973741, 4294967291};  

ErrorCode table_expansion(Table** table){
    unsigned prev_cap = (*table)->capacity;
    Table* new = malloc(sizeof(Table));
    if (new == NULL)
        return ERROR_MEMORY_ALLOCATION;
    
    new->capacity = new_capacity(prev_cap);
    new->size = 0;
    new->keyspace = calloc(new->capacity, sizeof(KeySpace*));
    if (new->keyspace == NULL)
        return ERROR_MEMORY_ALLOCATION;
    for (unsigned i = 0; i < (*table)->capacity; i++){
        KeySpace* ptr = (*table)->keyspace[i];
        while (ptr != NULL){
            ErrorCode error = func_input(&new, ptr->key, ptr->information);
            if (error != SUCCESS)
            return error;
            ptr = ptr->next;
        }
    }
    erase_table(*table);
    (*table) = new;
    return SUCCESS;
}



unsigned new_capacity(unsigned current_capacity){
    for (unsigned i = 0; i < 31; i++){
        if (simple_numbers[i] == current_capacity && i != 30)
            return (simple_numbers[i+1]);
    }
    return (simple_numbers[30]);
}