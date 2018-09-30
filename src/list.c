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

void list_tests(void)
{
    List* list = make_list();

    int* num = xmalloc(sizeof(int) * 10);
    num[0] = 1;
    num[1] = 2;
    num[2] = 3;
    num[3] = 4;
    num[4] = 5;
    num[5] = 6;

    num[6] = 53;
    num[7] = 5;

    // Append
    for (int i = 0; i < 6; ++i) {
        list_append(list, num[i]);
        assert((int)list_last(list) == num[i]);
    }

    // List at
    assert((int)list_last(list) == 6);
    assert((int)list_at(list, 3) == 4);

    // Prepend
    list_prepend(list, num[4] /* 5 */);
    list_prepend(list, num[6] /* 53 */);
    assert((int)list_at(list, 0) == 53);
    assert((int)list_at(list, 1) == 5);

    // Remove
    list_remove(list, 0);
    list_remove(list, 0);
    assert((int)list_at(list, 0) == 1);

    // // Uncomment to print the list
    // warning("List count: %d", list->count);
    // List_Node* current = list->head;
    // while (current != NULL) {
    //     warning("%d", (int)current->data);
    //     current = current->next;
    // }
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
    u64 iterator = 0;
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

    u64 iterator = 0;
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
