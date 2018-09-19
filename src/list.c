#include "list.h"
#include <assert.h>
#include <stdlib.h> // malloc
#include "utility.h" // warning

List* make_linked_list(void) {
    List* list = malloc(sizeof(List));
    list->head = NULL;
    return list;
}

void list_tests(void) {
    List* list = make_linked_list();

    list_insert(list, make_string("Hello!"));
    list_insert(list, make_string("My"));
    list_insert(list, make_string("name"));
    list_insert(list, make_string("is"));
    list_insert(list, make_string("Marcus"));
    list_insert(list, make_string("Mathiassen"));

    List_Node* current = list->head;

    while (current != NULL) {
        warning("element: %s", current->element->c_str);
        current = current->next;
    }
}

list_element_t list_insert(List* list, list_element_t element) {
    assert(list);
    List_Node* new_node = malloc(sizeof(List_Node));
    new_node->element = element;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
        return element;
    }

    List_Node* current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = new_node;
    return element;
}

list_element_t list_last(List* list) {
    assert(list);
    List_Node* current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    return current->element;
}
