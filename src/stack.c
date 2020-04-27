typedef struct Stack_Node {
    void* data;
    struct Stack_Node* next;
} Stack_Node;

typedef struct {
    struct Stack_Node* head;
    s64 count;
} Stack;

#define stack_foreach(s) for (Stack_Node* it = s->head; it; it = it->next)

INTERNAL Stack* make_stack()
{
    Stack* s = xmalloc(sizeof(Stack));
    s->head = NULL;
    s->count = 0;
    return s;
}

INTERNAL void* stack_push(Stack* stack, void* data)
{
    xassert(stack && data);
    Stack_Node* tmp = xmalloc(sizeof(Stack_Node));
    tmp->data = data;
    tmp->next = stack->head;
    stack->head = tmp;
    ++stack->count;
    return data;
}

INTERNAL void* stack_pop(Stack* stack)
{
    xassert(stack);
    if (!stack->head) return NULL;
    Stack_Node* tmp = stack->head;
    void* popped = tmp->data;
    stack->head = stack->head->next;
    --stack->count;
    free(tmp);
    return popped;
}

INTERNAL void* stack_peek(Stack* stack)
{
    xassert(stack);
    return stack->head->data;
}
