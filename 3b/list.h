#ifndef LIST_H
#define LIST_H

#include "library.h"
void input_current(Table**, unsigned, KeySpace*);//вставка текущего элемента
ErrorCode individuality(const Table*, const unsigned, const unsigned);
unsigned size_count(const KeySpace*);//считаем количество коллизий

#endif
