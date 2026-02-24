#ifndef QUE_H
#define QUE_H

#include "error.h"
typedef struct Queue{
    char** data;
    unsigned head;
    unsigned tail;
    unsigned capacity;
}Queue;//очередь для занесения всех ключей для удаления


ErrorCode init_queue(Queue*, unsigned);//инициализация очереди
ErrorCode enqueue(Queue*, const char*);//вставка в очередь 
char* dequeue(Queue*);//удаление из очереди
void free_queue(Queue*);//освобождение очереди

#endif