#ifndef LIST_H
#define LIST_H

#include "typedefs.h"

typedef struct List      List;
typedef struct List_Node List_Node;

struct List_Node {
    void*      data;
    List_Node* next;
    List_Node* prev;
};

struct List {
    List_Node* head;
    List_Node* tail;
    s64        count;
};

#define LIST_FOREACH_REVERSE(list) for (List_Node* it = (list->tail); (it); (it) = (it)->prev)
#define LIST_FOREACH(list) for (List_Node* it = (list->head); (it); (it) = (it)->next)

List* make_list(void);
void  list_free(List* l);
void  list_tests(void);
void* list_append(List* list, void* data);
void* list_prepend(List* list, void* data);
void* list_last(List* list);
void* list_first(List* list);
void* list_at(List* list, s64 index);
void  list_insert_after(List* list, List_Node* prev_node, void* data);
void  list_insert_before(List* list, List_Node* next_node, void* data);
void  list_append_content_of(List* list, List* other_list);
void  list_append_content_of_in_reverse(List* list, List* other_list);
void  list_prepend_content_of(List* list, List* other_list);
void  list_prepend_content_of_in_reverse(List* list, List* other_list);
void* list_remove_at(List* list, s64 index);
void* list_remove(List* list, List_Node* node);
bool  list_empty(List* list);

#endif
