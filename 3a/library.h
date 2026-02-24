#ifndef LIB_H
#define LIB_H

#include "error.h"

#include <stdio.h>
#define N 5//для capacity массива для функции search


typedef struct KeySpace{
    char* key;
    char* par;
    unsigned* information;
    struct KeySpace* next;
}KeySpace;

typedef struct Table{
    KeySpace* keyspace;
}Table;



//1 вставка элемента в таблицу
ErrorCode input_element(Table*, KeySpace*);//вставка элемента в таблицу
ErrorCode init_table(Table**);

void erase_table(Table*);//очистка таблицы


ErrorCode individuality(const Table*, const char*);//проверка на индивидуальность ключа
ErrorCode parent_check(const Table*, char*);//проверка, что родитель существует




//2 удаление элемента из таблицы по ключу (этот блок правильный)
ErrorCode delete(Table*, char*);








//3 поиск элемента по заданному ключу (этот блок правильный)
ErrorCode key_search(const Table*, char*, KeySpace**);//поиск ключа




//4 вывод таблицы с стдаут
ErrorCode output(const Table*, FILE*);


//5 считывание с файла
ErrorCode file_input(Table*, char*);



//6 индивид функция удаления
ErrorCode delete_elem_and_kids_null(Table*, char*);



//7 индивид функция с диапазоном
ErrorCode individual_search(const Table*, Table**, const char*, const char*);



#endif
