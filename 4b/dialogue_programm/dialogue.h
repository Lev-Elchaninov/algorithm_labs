#ifndef DIA_H
#define DIA_H

#include "error.h"
#include "library.h"


ErrorCode dialogue(LLRB_tree*);//диалог
void greeting();//приглащение

void errors_printf(ErrorCode);//функция вывода ошибок
char* input_filename();



void output_data(const Data*);
ErrorCode not_all();
ErrorCode entry(Data**, Node*);//вставка в массив для функции поиска



ErrorCode get_int(int*);//считка int
ErrorCode get_size_t(size_t*);//считка size_t
ErrorCode input_action(size_t*);//диалоговая

void pre_order_output(For_order*, FILE*);//вывод в прямом порядке следования ключей




ErrorCode operation_insert(LLRB_tree*);
ErrorCode operation_pre_order(LLRB_tree*);
ErrorCode operation_delete(LLRB_tree*);
ErrorCode operation_search(LLRB_tree*);

ErrorCode operation_search_min_exceeding(LLRB_tree*);

ErrorCode operation_load_from_txt(LLRB_tree**);
ErrorCode operation_output_to_graphviz(LLRB_tree*);

#endif