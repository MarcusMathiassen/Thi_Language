#ifndef STACK_H
#define STACK_H

#include "typedefs.h"
typedef struct Stack Stack;
typedef struct _StackNode _StackNode;

struct _StackNode {
    void* data;
    _StackNode* next;
};

struct Stack {
    _StackNode* head;
};

Stack* make_stack(void);
void* stack_push(Stack* stack, void* data);
void* stack_pop(Stack* stack);
void stack_tests(void);
#endif
