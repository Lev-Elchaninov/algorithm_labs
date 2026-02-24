#ifndef DIA_H
#define DIA_H

#include "error.h"
#include "library.h"


ErrorCode dialogue(Table*);//диалог
void greeting();//приглащение
/*ErrorCode init_table(Table**);//инициализация таблицы*/
/*void erase_table(Table*);//очищение таблицы*/
void errors_printf(ErrorCode);//функция вывода ошибок
char* input_filename();
void output_data(const Data*);
void erase_data(Data*);
ErrorCode not_all();
ErrorCode entry(Data**, Data_element);//вставка в массив для функции поиска

ErrorCode get_int(int*);//считка int
ErrorCode get_unsigned(unsigned*);//считка unsigned
ErrorCode input_action(int*);//диалоговая


ErrorCode operation_input(Table**);
ErrorCode operation_delete(Table*);
ErrorCode operation_search(const Table*);
ErrorCode operation_output_bin(const Table*);
ErrorCode operation_input_bin(Table**);
#endif