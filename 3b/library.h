#ifndef LIB_H
#define LIB_H

#define BORDER_RATIO 0.7
#include <stdio.h>
#include "error.h"


  

typedef struct KeySpace{
    unsigned key;
    unsigned information;
    struct KeySpace* next;
}KeySpace;

typedef struct Table{
    KeySpace** keyspace;
    unsigned capacity;
    unsigned size;
}Table;

typedef struct Data_element{//из них состоит массив для search
    unsigned key;
    unsigned information;
}Data_element;

typedef struct Data{//информация о массиве
    unsigned capacity;
    unsigned size;
    Data_element* array;
}Data;

//1. Вставка в таблицу
ErrorCode func_input(Table**, const unsigned, const unsigned);
//4. Вывод на экран
ErrorCode func_output(const Table*, FILE*);

//3. Поиск ключей
ErrorCode func_key_search(const Table*, unsigned, Data_element*);
//2. Удаление
ErrorCode func_delete(Table*, const unsigned);

//6. Запись в бинарь
ErrorCode func_output_bin(const Table*, const char*);

//5. Чтение из бинаря
ErrorCode func_input_bin(Table**, const char*);

void erase_table(Table*);
ErrorCode init_table(Table**);



#endif