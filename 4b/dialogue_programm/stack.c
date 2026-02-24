#include "stack.h"
#include <stdlib.h>

void stack_init(Stack* stack) {
    stack->top = NULL;
}

bool empty_stack(Stack* stack) {
    return (stack->top == NULL);
}

ErrorCode push(Stack* stack, void* ptr) {
    StackNode* new_stack_node = malloc(sizeof(StackNode));
    if (new_stack_node == NULL)
        return ERROR_MEMORY_ALLOCATION;
    new_stack_node->node = ptr;
    new_stack_node->next = stack->top;
    stack->top = new_stack_node;
    return SUCCESS;
}

ErrorCode pop(Stack* stack, void** ptr) {
    if (empty_stack(stack))
        return ERROR_EMPTY_STACK;
    StackNode* tmp = stack->top;
    *ptr = tmp->node;
    stack->top = stack->top->next;
    free(tmp);
    return SUCCESS;
}

void erase_stack(Stack* stack) {
    while (!empty_stack(stack)) {
        void* tmp;
        pop(stack, &tmp);
        free(tmp);
    }
}