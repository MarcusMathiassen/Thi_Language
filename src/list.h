// Copyright (c) 2019 Marcus Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef LIST_H
#define LIST_H

#include "typedefs.h"

//------------------------------------------------------------------------------
//                               list.h
//------------------------------------------------------------------------------

typedef struct List_Node_t {
    void* data;
    struct List_Node_t* next;
    struct List_Node_t* prev;
} List_Node;

typedef struct {
    List_Node* head;
    List_Node* tail;
    s64 count;
} List;

#define LIST_FOREACH_REVERSE(list) for (List_Node* it = (list->tail); (it); (it) = (it)->prev)
#define LIST_FOREACH(list) for (List_Node* it = (list->head); (it); (it) = (it)->next)

List* make_list(void);
void list_free(List* l);
void list_tests(void);
void list_sort(List* l, bool (*func)(void*, void*));
void* list_append(List* list, void* data);
void* list_prepend(List* list, void* data);
void* list_last(List* list);
void* list_first(List* list);
void* list_at(List* list, s64 index);
void list_insert_after(List* list, List_Node* prev_node, void* data);
void list_insert_before(List* list, List_Node* next_node, void* data);
void list_append_content_of(List* list, List* other_list);
void list_append_content_of_in_reverse(List* list, List* other_list);
void list_prepend_content_of(List* list, List* other_list);
void list_prepend_content_of_in_reverse(List* list, List* other_list);
void* list_remove_at(List* list, s64 index);
void* list_remove(List* list, List_Node* node);
bool list_empty(List* list);

#endif
