#ifndef LIB_H
#define LIB_H

#include "error.h"
#include <stdio.h>

typedef enum ColourCode{
    BLACK = 0,
    RED = 1
}ColourCode;

#define ONLY_DUPLICATES 1
#define ALL 0


typedef struct Info{//структура, в которой содержится информация
    size_t key;
    char* info;
    size_t release;//версия элемента
}Info;


typedef struct Node{//структура узла дерева
    Info info_fieldes;
    ColourCode colour;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    struct Node* duplicate_next;
}Node;

typedef struct For_order{
    size_t capacity;
    Info* array;
}For_order;



typedef struct Output_info{//нужно для вывода дерева на экран
    Node* node;
    int level;
    int dir;
}Output_info;



typedef struct LLRB_tree{//структура, определяющая характеристики бин. дерева
    Node* root;//указатель на корень дерева
    size_t number_of_elements;//количество элементов в дереве
}LLRB_tree;


typedef struct Data{//структура для поиска
    Node** storage;//массив указателей на желементы дерева
    size_t capacity;
    size_t size;
}Data;

extern Node* const ELIST;


ErrorCode init_tree(LLRB_tree**);
ErrorCode erase_tree(LLRB_tree*);

ErrorCode data_init(Data**);
void erase_data(Data*);


ErrorCode func_insert(LLRB_tree*, const size_t key, const char* info);
ErrorCode func_delete(LLRB_tree*, const size_t key);
ErrorCode func_output(const LLRB_tree*, FILE*);//сделал
ErrorCode func_pre_order(const LLRB_tree* tree, For_order** result, size_t*);
ErrorCode func_find_key(const LLRB_tree*, const size_t key, Node**);//функция поиска элемента, сделал
ErrorCode func_search_min_exceeding(const LLRB_tree* tree, const size_t key, Node** result);
ErrorCode func_load_from_txt(LLRB_tree** tree, const char* filename);//из текстового считывание, сделал
ErrorCode func_output_to_graphviz(const LLRB_tree*);//сделал


void order_array_free(For_order*);//очистка массива под обходы, сделал

#endif