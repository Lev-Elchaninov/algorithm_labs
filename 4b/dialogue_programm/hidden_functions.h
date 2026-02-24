#ifndef HID_H
#define HID_H

#include "stack.h"
#include "library.h"
#include <stdio.h>

ErrorCode node_init(Node** current, const size_t key, const char* info);

ErrorCode char_input_txt(FILE* file, char** current);

ErrorCode data_array_creation(Data**, const LLRB_tree*, int);

void colour_flip(Node*);
int restoring_the_properties(Node** parent_ptr, LLRB_tree* tree);//функция для восстановления свойств дерева

//случай 1: ключ совпал и он в левом поддереве, есть родитель
ErrorCode key_math_in_left_tree_parent_exists(Node* ptr, Node** result);

//случай 2: ключ не совпал и он меньше, чем его родитель
ErrorCode key_not_found_and_less_than_parent(Node* ptr, Node** result);

//случай 3: ключ совпал и это корень
ErrorCode key_match_and_is_root(Node* ptr, Node** result);

//случай 4: ключ совпал и он в правом поддереве его родителя
ErrorCode key_math_in_right_tree_parent_exists(Node* ptr, Node** result);

//случай 5: ключ не совпал и он должен быть в правом поддереве от своего родителя
ErrorCode key_not_found_and_more_than_parent(Node* ptr, Node** result, const size_t key);


void replace_node(Node**, Node**);//поменять нодик на его дубликат



void pre_order_insert_in_array(Node**, For_order*, size_t*, int*);//функция для декомпозиции


Node* recursive_delete(LLRB_tree*, Node*, const size_t);//
Node* move_red_left(Node*, LLRB_tree* tree);//
Node* move_red_right(Node*, LLRB_tree* tree);//
Node* min(Node*);
Node* rotate_left(Node*, LLRB_tree*);
Node* rotate_right(Node*, LLRB_tree*);
Node* fix_up(Node*, LLRB_tree*);//тут восстановление свойств в другом порядке
Node* delete_min(Node*, LLRB_tree*);//
#endif