#ifndef ERROR_H
#define ERROR_H

typedef struct Error_storage{
    char* string;
    unsigned error_code;
}Error_storage;


typedef enum{
    ERROR_EMPTY_GRAPH = 0,
    ERROR_EOF = 1,
    ERROR_VERTEX_EXIST= 2,
    ERROR_UNKNOWN_VERTEX = 3,
    ERROR_NOT_SIZE_T = 4,
    ERROR_OPENING_FILE = 5,
    ERROR_EMPTY_EDGE = 6,
    ERROR_NULL_HOST = 7,
    ERROR_EDGE_EXIST = 8,
    ERROR_UNKNOWN_EDGE = 9,//только что добавил
    ERROR_NO_PATH = 10,
    ERROR_CANT_BUILD_MST = 11,
    SUCCESS = 12,

    ERROR_INVALID_PARAMETER = 13,
    ERROR_MEMORY_ALLOCATION = 14,
    ERROR_NOT_ALL = 15,
    ERROR_EMPTY_STACK = 16
}ErrorCode;



#endif