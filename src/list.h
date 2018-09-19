#ifndef LIST_H
#define LIST_H

#include "typedefs.h"
#include "string.h"

typedef struct List List;
typedef struct List_Node List_Node;

typedef string* list_element_t;

struct List_Node {
    list_element_t element;
    List_Node* next;
};

struct List {
    List_Node* head;
};

list_element_t list_insert(List* list, list_element_t element);
List* make_linked_list(void);
list_element_t list_last(List* list);
void list_tests(void);

#endif
