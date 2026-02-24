#ifndef TALK_H
#define TALK_H

#include "error.h"
#include "library.h"

typedef struct Element{//из них создается массив (операция - поиск)
    char* key;
    unsigned* information;
}Element;


typedef struct Data{//для функции 3 (поиск)
    Element* storage;
    unsigned size;
    unsigned capacity;
}Data;


//диалог
ErrorCode dialogue(Table*);
void greeting();
ErrorCode input_action(int*);
ErrorCode getint(int*);
ErrorCode get_unsigned(int*);
ErrorCode key_input(char**);

//проверка на пустую таблицу и функция ошибок 
ErrorCode empty_table_check(const Table*);
void errors_printf(ErrorCode);

//3 поиск элемента по заданному ключу
void out_data(const Data*);//вывод содержимого массива
void erase_data(Data*);//очистка массива
ErrorCode entry(Data**, const KeySpace*);//сама функция вставки двойной указатель - потому что realloc

ErrorCode par_input(const Table*, KeySpace**);
ErrorCode info_input(KeySpace*);

//1 занесение в таблицу
ErrorCode element_entry(const Table*, KeySpace*);//считывание элемента для функции 1




//операции
ErrorCode operation_input(Table*);
ErrorCode operation_delete(Table*);
ErrorCode operation_search(const Table*);
ErrorCode operation_file_input(Table*);
ErrorCode operation_delete_elem_and_kids_null(Table*);
ErrorCode operation_search_in_range(const Table*);

//5 считывание с файла
char* input_filename();//считывает содержимое файла
ErrorCode not_all();//спрашиваем, все ли ключи ввел пользователь


//7 поиск элемента в диапазоне
ErrorCode borders_input(char**, char**);


#endif