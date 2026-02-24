#ifndef ERROR_H
#define ERROR_H

typedef struct Error_storage{
    char* string;
    unsigned error_code;
}Error_storage;




typedef enum{
    ERROR_EMPTY_TABLE = 0,
    ERROR_EOF = 1,
    ERROR_KEY_MATCH = 2,
    ERROR_PARENT_NOT_FOUND = 3,
    ERROR_KEY_NOT_FOUND = 4,
    ERROR_NOT_UNSIGNED = 5,
    ERROR_OPENING_FILE = 6,
    ERROR_FORBIDDEN_KEY = 7,
    SUCCESS = 8,
    ERROR_NULL_POINTER = 9,
    ERROR_MEMORY_ALLOCATION = 10,
    ERROR_NOT_ALL = 11
}ErrorCode;



#endif