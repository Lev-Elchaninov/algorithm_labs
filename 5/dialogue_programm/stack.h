#ifndef STACK_H
#define STACK_H

#include "library.h"
#include <stdbool.h>


typedef struct StackNode{//узел списка
    struct ArrayElem* node;
    struct StackNode* next;
}StackNode;

typedef struct Stack{//стек
    StackNode* top;
}Stack;


void stack_init(Stack*);//инит стека
bool empty_stack(Stack* stack);//проверка на то, что стек пуст
ErrorCode push(Stack* stack, void*);//вставка в стек
ErrorCode pop(Stack* stack, void**);//выборка из стека
void erase_stack(Stack*);//очистка всех стеков (под узлы, под уровни и под направление)

#endif