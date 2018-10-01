#include "list.h"
#include "utility.h" // error, xmalloc
#include <assert.h>
#include <stdlib.h> // malloc

List* make_list(void)
{
    List* list = xmalloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

typedef struct
{
    int id;
    float val;
} Test_Type;


void list_tests(void)
{
    List* list = make_list();

    Test_Type marcus;
    marcus.id = 0;
    marcus.val = 3.43f;

    Test_Type aylin;
    aylin.id = 1;
    aylin.val = 6.41f;

    // Append
    list_append(list, &marcus);
    list_append(list, &aylin);

    // At
    assert(((Test_Type*)list_at(list, 0))->val == 3.43f);
    assert(((Test_Type*)list_last(list))->val == 6.41f);

    // // Prepend
    // list_prepend(list, num[4] /* 5 */);
    // list_prepend(list, num[6] /* 53 */);
    // assert((int)list_at(list, 0) == 53);
    // assert((int)list_at(list, 1) == 5);

    // // Remove
    // list_remove(list, 0);
    // list_remove(list, 0);
    // assert((int)list_at(list, 0) == 1);

    // Uncomment to print the list
    info("List count: %d", list->count);
    LIST_FOREACH(list) {
        Test_Type *tp = (Test_Type*)it->data;
        info("Test_Type id: %d, val: %f", tp->id, tp->val);
    }
}

void* list_remove(List* list, i64 index)
{
    assert(list);
    assert(index >= 0);

    --list->count;

    if (index == 0) {
        if (list->head->next) list->head = list->head->next;
        // free(list->head);
        return list->head->data;
    }
    i64 iterator = 0;
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

void* list_at(List* list, i64 index)
{
    assert(list);
    assert(index >= 0);

    i64 iterator = 0;
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

    ++list->count;

    List_Node* new_node = xmalloc(sizeof(List_Node));
    new_node->data = data;

    if (list->head == NULL) {
        new_node->next = NULL;
        list->head = new_node;
        return data;
    }

    new_node->next = list->head;
    list->head = new_node;

    return data;
}

void* list_append(List* list, void* data)
{
    assert(list);

    ++list->count;

    List_Node* new_node = xmalloc(sizeof(List_Node));
    new_node->data = data;
    new_node->next = NULL;

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
