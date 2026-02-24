#ifndef HID_H
#define HID_H

#include "stack.h"
#include "library.h"
#include <stdio.h>

ErrorCode node_init(Node** current, const size_t key, const char* info);

ErrorCode char_input_txt(FILE* file, char** current);

ErrorCode func_pre_order(const Bin_tree*, For_order**);//прямой обход дерева
ErrorCode data_array_creation(Data**, const Bin_tree*);

#endif 