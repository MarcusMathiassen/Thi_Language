#include "list.h"
#include "stack.h"
#include "utility.h" // error, xmalloc
#include <assert.h>
#include <stdlib.h> // xmalloc

List* make_list()
{
    List* l = xmalloc(sizeof(List));
    list_init(l);
    return l;
}

void list_init(List* list)
{
    assert(list);
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

typedef struct
{
    char* name;
    float val;
} Test_Type;

void list_tests(void)
{
    List* list = make_list();

    Test_Type t1;
    t1.name = "t1";
    t1.val = 3.43f;

    Test_Type t2;
    t2.name = "t2";
    t2.val = 6.41f;

    // Append
    list_append(list, &t1);
    list_append(list, &t2);


    // At
    assert(((Test_Type*)list_at(list, 0))->val == 3.43f);
    assert(((Test_Type*)list_last(list))->val == 6.41f);

    // // Prepend
    list_prepend(list, &t2);
    assert(((Test_Type*)list_at(list, 0))->val == 6.41f);

    // Remove
    list_remove_at(list, 0);
    assert(((Test_Type*)list_at(list, 0))->val == 3.43f);
    assert(((Test_Type*)list_at(list, 1))->val == 6.41f);

    Test_Type t3;
    t3.name = "t3";
    t3.val = 7.43f;

    list_append_after(list, list->head, &t3);
    assert(((Test_Type*)list_at(list, 1))->val == 7.43f);

    list_append_before(list, list->tail, &t3);
    assert(((Test_Type*)list_at(list, list->count-1))->val == 7.43f);

    // Uncomment to print the list
    // info("List count: %d", list->count);
    // LIST_FOREACH(list)
    // {
    //     Test_Type* tp = (Test_Type*)it->data;
    //     warning("Test_Type name: %s, val: %f", tp->name, tp->val);
    // }
}

bool list_empty(List* list) { return list->count; }
void list_append_content_of_in_reverse(List* list, List* other_list)
{
    assert(list);
    assert(other_list);
    Stack* s = make_stack();
    LIST_FOREACH(other_list) { stack_push(s, it->data); }
    LIST_FOREACH(other_list) { list_append(list, stack_pop(s)); }
    stack_free(s);
}
void list_append_content_of(List* list, List* other_list)
{
    assert(list);
    assert(other_list);
    LIST_FOREACH(other_list) { list_append(list, it->data); }
}

void* list_remove_at(List* list, s64 index)
{
    assert(list);
    assert(index >= 0);
    --list->count;
    if (index == 0) {
        if (list->head->next) list->head = list->head->next;
        // free(list->head);
        return list->head->data;
    }
    s64 iterator = 0;
    List_Node* current = list->head;
    List_Node* prev = current;
    while (current->next != NULL && iterator != index) {
        ++iterator;
        prev = current;
        current = current->next;
    }
    if (current->next == NULL) {
        error("No data in list with index %d", index);
    }
    prev->next = current->next;
    // free(current);
    return current->data;
}

void* list_remove(List* list, List_Node* node)
{
    assert(list);
    assert(node);
    assert(list->count > 0);
    assert(list->head);
    assert(list->tail);

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    list->count -= 1;
    return node;
}

void* list_at(List* list, s64 index)
{
    assert(list);
    assert(index >= 0);
    s64 iterator = 0;
    List_Node* current = list->head;
    while (current->next != NULL && iterator != index) {
        ++iterator;
        current = current->next;
    }
    if (iterator != index) {
        error("No data in list with index %d", index);
    }
    return current->data;
}

void* list_prepend(List* list, void* data)
{
    assert(list);
    assert(data);
    ++list->count;
    List_Node* new_node = xmalloc(sizeof(List_Node));
    new_node->data = data;
    new_node->prev = NULL;
    if (list->head == NULL) {
        new_node->next = NULL;
        list->head = new_node;
        return data;
    }
    new_node->next = list->head;
    new_node->prev = new_node;
    list->head = new_node;
    return data;
}

void* list_append(List* list, void* data)
{
    assert(list);
    assert(data);
    ++list->count;
    List_Node* new_node = xmalloc(sizeof(List_Node));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = list->tail;
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = list->head;
        return data;
    }
    list->tail->next = new_node;
    list->tail = new_node;
    return data;
}

void* list_first(List* list)
{
    assert(list);
    return list->head->data;
}

void* list_last(List* list)
{
    assert(list);
    return list->tail->data;
}

void list_append_after(List* list, List_Node* prev_node, void* data) 
{ 
    assert(prev_node);
    List_Node* new_node = xmalloc(sizeof(List_Node));
    new_node->data = data;
    new_node->next = prev_node->next;
    new_node->prev = prev_node; 
    prev_node->next = new_node; 
    if (new_node->next) {
        new_node->next->prev = new_node;
    } else {
        list->tail = new_node;
    }
} 

void list_append_before(List* list, List_Node* next_node, void* data) 
{ 
    assert(next_node);
    List_Node* new_node = xmalloc(sizeof(List_Node));
    new_node->data = data;
    new_node->next = next_node;
    new_node->prev = next_node->prev; 
    next_node->prev = new_node; 
    if (new_node->prev) {
        new_node->prev->next = new_node;
    } else {
        list->head = new_node;
    }
} 
