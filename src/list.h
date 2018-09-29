#ifndef LIST_H
#define LIST_H

#include "typedefs.h"

typedef struct List_Node {
    void* element;
    struct List_Node* next;
} List_Node;

typedef struct List {
    List_Node* head;
    List_Node* tail;
    u64 count;
} List;

void list_tests(void);
List* make_list(void);
void* list_append(List* list, void* element);
void* list_prepend(List* list, void* element);
void* list_last(List* list);
void* list_first(List* list);
void* list_at(List* list, i64 index);
void* list_remove(List* list, i64 index);

#endif
