#include "stack.h"
#include "utility.h" // xmalloc, warning

#include <assert.h>

Stack* make_stack(void)
{
    Stack* stack = xmalloc(sizeof(Stack));
    stack->head = NULL;
    return stack;
}

typedef struct
{
    int id;
    float val;
} Test_Type;

void stack_tests(void)
{
    Stack* test_stack = make_stack();
    Test_Type* marcus = xmalloc(sizeof(Test_Type));
    marcus->id = 0;
    marcus->val = 3.43f;

    Test_Type* aylin = xmalloc(sizeof(Test_Type));
    aylin->id = 1;
    aylin->val = 6.41f;

    stack_push(test_stack, marcus);
    stack_push(test_stack, aylin);
    assert(((Test_Type*)stack_pop(test_stack))->val == 6.41f);
    assert(((Test_Type*)stack_pop(test_stack))->val == 3.43f);
    assert((Test_Type*)stack_pop(test_stack) == NULL);
}

void* stack_push(Stack* stack, void* data)
{
    assert(stack);
    assert(data);
    _StackNode* tmp = xmalloc(sizeof(Stack));
    tmp->data = data;
    tmp->next = stack->head;
    stack->head = tmp;
    return data;
}

void* stack_pop(Stack* stack)
{
    assert(stack);
    if (!stack->head) return NULL;
    _StackNode* tmp = stack->head;
    void* popped = tmp->data;
    stack->head = stack->head->next;
    free(tmp);
    return popped;
}

void* stack_peek(Stack* stack) { return stack->head->data; }
