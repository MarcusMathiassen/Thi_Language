#ifndef LIST_H
#define LIST_H

#include "typedefs.h"

typedef struct List List;
typedef struct List_Node List_Node;

typedef int list_element_t;

struct List_Node {
    list_element_t element;
    List_Node* next;
};

struct List {
    List_Node* head;
    List_Node* tail;
};

void list_tests(void);
List* make_linked_list(void);
list_element_t list_append(List* list, list_element_t element);
list_element_t list_prepend(List* list, list_element_t element);
list_element_t list_last(List* list);
list_element_t list_first(List* list);
list_element_t list_at(List* list, i64 index);
void list_remove(List* list, i64 index);

#endif
