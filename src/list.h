#ifndef LIST_H
#define LIST_H

#include "typedefs.h"

typedef struct List_Node
{
    void* data;
    struct List_Node* next;
} List_Node;

typedef struct List
{
    List_Node* head;
    List_Node* tail;
    s64 count;
} List;

#define LIST_FOREACH(list) for (List_Node* it = (list->head); (it); (it) = (it)->next)

List* make_list();
void list_tests(void);
void list_init(List* list);
void* list_append(List* list, void* data);
void* list_prepend(List* list, void* data);
void* list_last(List* list);
void* list_first(List* list);
void* list_at(List* list, s64 index);
void list_append_content_of(List* list, List* other_list);
void list_append_content_of_in_reverse(List* list, List* other_list);
void* list_remove(List* list, s64 index);
bool list_empty(List* list);

#endif
