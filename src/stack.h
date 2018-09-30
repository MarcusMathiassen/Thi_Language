#ifndef STACK_H
#define STACK_H

#include "typedefs.h"
typedef struct Stack Stack;
typedef struct Stack_Node Stack_Node;

struct Stack_Node
{
    void* data;
    Stack_Node* next;
};

struct Stack
{
    Stack_Node* head;
};

#define STACK_FOREACH(stack) for (Stack_Node* it = (stack->head); (it); (it) = (it)->next)

Stack* make_stack(void);
void* stack_push(Stack* stack, void* data);
void* stack_pop(Stack* stack);
void* stack_peek(Stack* stack);
void stack_tests(void);
#endif
