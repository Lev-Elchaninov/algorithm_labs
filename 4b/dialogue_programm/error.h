#ifndef ERROR_H
#define ERROR_H

typedef struct Error_storage{
    char* string;
    unsigned error_code;
}Error_storage;


typedef enum{
    ERROR_EMPTY_TREE = 0,
    ERROR_EOF = 1,
    ERROR_KEY_MATCH = 2,
    ERROR_PARENT_NOT_FOUND = 3,
    ERROR_KEY_NOT_FOUND = 4,
    ERROR_NOT_SIZE_T = 5,
    ERROR_OPENING_FILE = 6,
    ERROR_EMPTY_ORDER = 7,
    SUCCESS = 8,
    ERROR_MEMORY_ALLOCATION = 9,
    ERROR_NOT_ALL = 10,
    ERROR_NULL_INFO = 11,
    ERROR_EMPTY_STACK = 12
}ErrorCode;



#endif