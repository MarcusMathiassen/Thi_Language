// Copyright (c) 2019 Marcus Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "stack.h"
#include "utility.h" // xmalloc, warning


#include <stdlib.h> // free

Stack*
make_stack() {
    Stack* s = xmalloc(sizeof(Stack));
    stack_init(s);
    return s;
}

void stack_init(Stack* stack) {
    stack->head = NULL;
    stack->count = 0;
}
void stack_free(Stack* stack) {
    while (stack_pop(stack) != NULL) {
    }
}
typedef struct
{
    s32 id;
    float val;
} Test_Type;

void stack_tests(void) {
    Stack test_stack;
    stack_init(&test_stack);
    Test_Type t1;
    t1.id = 0;
    t1.val = 3.43f;

    Test_Type t2;
    t2.id = 1;
    t2.val = 6.41f;

    stack_push(&test_stack, &t1);
    xassert(((Test_Type*)stack_peek(&test_stack))->id == 0);
    stack_push(&test_stack, &t2);
    xassert(((Test_Type*)stack_peek(&test_stack))->id == 1);

    xassert(((Test_Type*)stack_pop(&test_stack))->val == 6.41f);
    xassert(((Test_Type*)stack_pop(&test_stack))->val == 3.43f);
    xassert((Test_Type*)stack_pop(&test_stack) == NULL);
}

void* stack_push(Stack* stack, void* data) {
    xassert(stack);
    xassert(data);
    Stack_Node* tmp = xmalloc(sizeof(Stack_Node));
    tmp->data = data;
    tmp->next = stack->head;
    stack->head = tmp;
    ++stack->count;
    return data;
}

void* stack_pop(Stack* stack) {
    xassert(stack);
    if (!stack->head) return NULL;
    Stack_Node* tmp = stack->head;
    void* popped = tmp->data;
    stack->head = stack->head->next;
    --stack->count;
    free(tmp);
    return popped;
}

void* stack_peek(Stack* stack) {
    return stack->head->data;
}
