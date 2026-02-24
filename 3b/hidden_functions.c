#include "hidden_functions.h"


unsigned hash_function(const unsigned key, const unsigned capacity){
    const double A = 0.6180339887; //Кнут вычислил идеальную A

    //Выделяю дробную часть от key * A
    double multiple = key * A;
    double fract_part = multiple - (unsigned)multiple; 

    //умножаем на длину массива
    unsigned hash = (unsigned)(capacity * fract_part);
    return hash;
}




ErrorCode individuality(const Table* table, const unsigned hash, const unsigned key){
    if (table == NULL)
        return ERROR_EMPTY_TABLE;
    
    KeySpace* ptr = table->keyspace[hash];
    while (ptr != NULL){
        if (ptr->key == key)
            return ERROR_KEY_MATCH;
        ptr = ptr->next;
    }
    return SUCCESS;
}






void input_current(Table** table, unsigned i, KeySpace* current){
    if ((*table)->keyspace[i] == NULL){
        (*table)->keyspace[i] = current;
        current->next = NULL;
    }
    else if ((*table)->keyspace[i] != NULL){
        current->next = (*table)->keyspace[i];
        (*table)->keyspace[i] = current;
    }
}




unsigned size_count(const KeySpace* current){
    unsigned size = 0;
    while (current != NULL){
        size++;
        current = current->next;
    }
    return size;
}
