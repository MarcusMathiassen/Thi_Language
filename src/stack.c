#include "stack.h"
#include "utility.h" // xmalloc, warning

#include <assert.h>
#include <stdlib.h> // free

void stack_init(Stack* stack) { stack->head = NULL; }

typedef struct
{
    i32 id;
    float val;
} Test_Type;

void stack_tests(void)
{
    Stack test_stack;
    stack_init(&test_stack);
    Test_Type t1;
    t1.id = 0;
    t1.val = 3.43f;

    Test_Type t2;
    t2.id = 1;
    t2.val = 6.41f;

    stack_push(&test_stack, &t1);
    stack_push(&test_stack, &t2);
    assert(((Test_Type*)stack_pop(&test_stack))->val == 6.41f);
    assert(((Test_Type*)stack_pop(&test_stack))->val == 3.43f);
    assert((Test_Type*)stack_pop(&test_stack) == NULL);
}

void* stack_push(Stack* stack, void* data)
{
    assert(stack);
    assert(data);
    Stack_Node* tmp = xmalloc(sizeof(Stack));
    tmp->data = data;
    tmp->next = stack->head;
    stack->head = tmp;
    return data;
}

void* stack_pop(Stack* stack)
{
    assert(stack);
    if (!stack->head) return NULL;
    Stack_Node* tmp = stack->head;
    void* popped = tmp->data;
    stack->head = stack->head->next;
    free(tmp);
    return popped;
}

void* stack_peek(Stack* stack) { return stack->head->data; }
