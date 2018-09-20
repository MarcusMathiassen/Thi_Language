#include "list.h"
#include "utility.h" // error
#include <assert.h>
#include <stdlib.h> // malloc

List* make_linked_list(void) {
    List* list = malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void list_tests(void) {

    List* list = make_linked_list();

    // Append
    list_append(list, 1);
    assert(list_last(list) == 1);
    list_append(list, 2);
    assert(list_last(list) == 2);
    list_append(list, 3);
    assert(list_last(list) == 3);
    list_append(list, 4);
    assert(list_last(list) == 4);
    list_append(list, 5);
    assert(list_last(list) == 5);
    list_append(list, 6);
    assert(list_last(list) == 6);
    assert(list_at(list, 3) == 4);

    // Prepend
    list_prepend(list, 5);
    list_prepend(list, 53);
    assert(list_at(list, 0) == 53);
    assert(list_at(list, 1) == 5);

    // Remove
    list_remove(list, 0);
    list_remove(list, 0);
    assert(list_at(list, 0) == 1);

    // Uncomment to print our the list
    // List_Node* current = list->head;
    // while (current != NULL) {
    //     warning("%d", current->element);
    //     current = current->next;
    // }
}

void list_remove(List* list, i64 index) {
    assert(list);
    assert(index >= 0);

    if (index == 0) {
        if (list->head->next)
            list->head = list->head->next;
        free(list->head);
        return;
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
        error("No element in list with index %d", index);
    }

    prev->next = current->next;
    free(current);
}

list_element_t list_at(List* list, i64 index) {
    assert(list);
    assert(index >= 0);

    u64 iterator = 0;
    List_Node* current = list->head;

    while (current->next != NULL && iterator != index) {
        ++iterator;
        current = current->next;
    }

    if (iterator != index) {
        error("No element in list with index %d", index);
    }
    return current->element;
}

list_element_t list_prepend(List* list, list_element_t element) {
    assert(list);
    List_Node* new_node = malloc(sizeof(List_Node));
    new_node->element = element;

    if (list->head == NULL) {
        new_node->next = NULL;
        list->head = new_node;
        return element;
    }

    new_node->next = list->head;
    list->head = new_node;

    return element;
}

list_element_t list_append(List* list, list_element_t element) {
    assert(list);
    List_Node* new_node = malloc(sizeof(List_Node));
    new_node->element = element;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
        list->tail = list->head;
        return element;
    }

    list->tail->next = new_node;
    list->tail = new_node;

    return element;
}

list_element_t list_first(List* list) {
    assert(list);
    return list->head->element;
}


list_element_t list_last(List* list) {
    assert(list);
    return list->tail->element;
}
