#ifndef LIB_H
#define LIB_H

#include "error.h"
#include <stdio.h>

typedef struct Info{//структура, в которой содержится информация
    size_t key;
    char* info;
}Info;


typedef struct Node{//структура узла дерева
    Info info_fieldes;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
}Node;


typedef struct Output_info{//нужно для вывода дерева на экран
    Node* node;
    int level;
    int dir;
}Output_info;



typedef struct For_order{
    size_t capacity;
    Info* array;
}For_order;


typedef struct Bin_tree{//структура, определяющая характеристики бин. дерева
    Node* root;//указатель на корень дерева
    size_t number_of_elements;//количество элементов в дереве
}Bin_tree;


typedef struct Data{//структура для поиска
    Node** storage;//массив указателей на желементы дерева
    size_t capacity;
    size_t size;
}Data;


ErrorCode init_tree(Bin_tree**);
ErrorCode erase_tree(Bin_tree*);

ErrorCode data_init(Data**);
void erase_data(Data*);


ErrorCode func_insert(Bin_tree*, const size_t key, const char* info);
ErrorCode func_delete(Bin_tree*, const size_t key);
ErrorCode func_output(const Bin_tree*, FILE*);
ErrorCode func_pre_order_in_range(const Bin_tree* tree, const size_t left_board, const size_t right_board, For_order** result);
ErrorCode func_find_key(const Bin_tree*, const size_t key, Node**);//функция поиска элемента
ErrorCode func_find_most_dif_value(const Bin_tree* tree, const size_t key, Data** data);
ErrorCode func_load_from_txt(Bin_tree** tree, const char* filename);//из текстового считывание
ErrorCode func_output_to_graphviz(const Bin_tree*);


void order_array_free(For_order*);//очистка массива под обходы

#endif