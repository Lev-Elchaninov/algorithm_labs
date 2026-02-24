#ifndef HID_H
#define HID_H

#include "library.h"

void free_keyspace(KeySpace*);//очищение нодика
ErrorCode add_key(char***, unsigned*, unsigned*, const char*);//добавление ключа в массив
void free_char_array(char**, unsigned);
ErrorCode char_input_txt(char**, FILE*, Table*, KeySpace*);//считывает строку с текстового файла

#endif