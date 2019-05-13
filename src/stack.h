#ifndef STACK_H
#define STACK_H

#include "typedefs.h"

typedef struct Stack_Node {
    void*              data;
    struct Stack_Node* next;
} Stack_Node;

typedef struct {
    struct Stack_Node* head;
} Stack;

#define STACK_FOREACH(stack) for (Stack_Node* it = (stack->head); (it); (it) = (it)->next)

Stack* make_stack();
void   stack_init(Stack* stack);
void   stack_free(Stack* stack);
void*  stack_push(Stack* stack, void* data);
void*  stack_pop(Stack* stack);
void*  stack_peek(Stack* stack);
void   stack_tests(void);
#endif
