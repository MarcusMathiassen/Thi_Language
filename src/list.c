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

#include "list.h"
#include "stack.h"
#include "utility.h" // error, xmalloc

#include <stdlib.h> // xmalloc

List* make_list(void) {
    List* l = xmalloc(sizeof(List));
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
    return l;
}

void list_init(List* l) {
    l = xmalloc(sizeof(List));
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
}

void list_free(List* l) {
    list_foreach(l) {
        free(it->prev);
    }
}

typedef struct
{
    char* name;
    float val;
} Test_Type;

void list_tests(void) {
    List* list = make_list();

    Test_Type t1;
    t1.name = "t1";
    t1.val = 3.43f;

    Test_Type t2;
    t2.name = "t2";
    t2.val = 6.41f;

    // Append
    list_append(list, &t1);
    list_append(list, &t2);

    // At
    xassert(((Test_Type*)list_first(list))->val == 3.43f);
    xassert(((Test_Type*)list_last(list))->val == 6.41f);

    // Prepend
    list_prepend(list, &t2);
    xassert(((Test_Type*)list_at(list, 0))->val == 6.41f);

    // Remove
    list_remove_at(list, 0);
    // Test_Type* t = (Test_Type*)list_at(list, 0);
    // xassert(t->val == 3.43f);
    // xassert(((Test_Type*)list_at(list, 1))->val == 6.41f);

    Test_Type t3;
    t3.name = "t3";
    t3.val = 7.43f;

    list_insert_after(list, list->head, &t3);
    xassert(((Test_Type*)list_at(list, 1))->val == 7.43f);

    list_insert_before(list, list->tail, &t3);
    xassert(((Test_Type*)list_at(list, list->count - 1))->val == 7.43f);

    list_free(list);
}

bool list_empty(List* list) {
    return (list->head == NULL && list->tail == NULL);
}

void list_prepend_content_of_in_reverse(List* list, List* other_list) {
    xassert(list);
    xassert(other_list);
    list_foreach_reverse(other_list) {
        list_prepend(list, it->data);
    }
}
void list_prepend_content_of(List* list, List* other_list) {
    xassert(list);
    xassert(other_list);
    list_foreach(other_list) {
        list_prepend(list, it->data);
    }
}

void list_append_content_of_in_reverse(List* list, List* other_list) {
    xassert(list);
    xassert(other_list);
    list_foreach_reverse(other_list) {
        list_append(list, it->data);
    }
}
void list_append_content_of(List* list, List* other_list) {
    xassert(list);
    xassert(other_list);
    list_foreach(other_list) {
        list_append(list, it->data);
    }
}

void* list_remove_at(List* list, s64 index) {
    xassert(list);
    xassert(index >= 0 && index <= list->count);

    bool start_from_tail = (list->count - index) < index ? true : false;

    List_Node* removed_node = NULL;
    s64 iterator = 0;

    if (start_from_tail) {
        iterator = list->count;
        list_foreach_reverse(list) {
            if (iterator == index) {
                removed_node = list_remove(list, it);
                break;
            }
            iterator -= 1;
        }
    } else {
        list_foreach(list) {
            if (iterator == index) {
                removed_node = list_remove(list, it);
                break;
            }
            iterator += 1;
        }
    }

    return removed_node->data;
}

void* list_remove_at_end(List* list) {
    xassert(list);
    if (list_empty(list)) {
        return NULL;
    }
    if (list->tail == list->head) {
        list->tail = NULL;
        list->head = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    return list->tail;
}

void* list_remove_at_start(List* list) {
    xassert(list);
    if (list_empty(list)) {
        return NULL;
    }
    if (list->tail == list->head) {
        list->tail = NULL;
        list->head = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    return list->head;
}

void* list_remove(List* list, List_Node* node) {
    xassert(list);
    xassert(node);

    if (list_empty(list)) {
        return NULL;
    }
    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else if (node == list->tail) {
        list_remove_at_end(list);
    } else if (node == list->head) {
        list_remove_at_start(list);
    } else {
        List_Node* current = list->head;
        while (current != node) {
            current = current->next;
        }
        current->prev->next = current->next;
        if (current->next != NULL) {
            current->next->prev = current->prev;
        }
        current = NULL;
    }

    list->count -= 1;
    return node;
}

void* list_at(List* list, s64 index) {
    xassert(list);
    xassert(index >= 0 && index <= list->count);

    bool start_from_tail = (list->count - index) < index ? true : false;
    void* data = NULL;

    if (start_from_tail) {
        s64 iterator = list->count;
        list_foreach_reverse(list) {
            if (iterator == index) {
                data = it->data;
                break;
            }
            iterator -= 1;
        }
    } else {
        s64 iterator = 0;
        list_foreach(list) {
            if (iterator == index) {
                data = it->data;
                break;
            }
            iterator += 1;
        }
    }
    return data;
}

void* list_prepend(List* list, void* data) {
    xassert(list);
    // xassert(data);
    if (!data) return data;
    ++list->count;
    List_Node* new_node = xmalloc(sizeof(List_Node));
    new_node->data = data;
    new_node->prev = NULL;
    if (list->head == NULL) {
        new_node->next = NULL;
        list->head = new_node;
        return data;
    }
    new_node->next = list->head;
    new_node->prev = new_node;
    list->head = new_node;
    return new_node->data;
}

void* list_append(List* list, void* data) {
    tassert(list && data, "%zu, %zu", list, data);
    // xassert(data);
    if (!data) return data;
    ++list->count;
    List_Node* new_node = xmalloc(sizeof(List_Node));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = list->tail;
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = list->head;
        return data;
    }
    list->tail->next = new_node;
    list->tail = new_node;
    return new_node->data;
}

void* list_first(List* list) {
    xassert(list);
    return list->head->data;
}

void* list_last(List* list) {
    xassert(list);
    return list->tail->data;
}

void list_insert_after(List* list, List_Node* prev_node, void* data) {
    xassert(prev_node);
    List_Node* new_node = xmalloc(sizeof(List_Node));
    new_node->data = data;
    new_node->next = prev_node->next;
    new_node->prev = prev_node;
    prev_node->next = new_node;
    if (new_node->next) {
        new_node->next->prev = new_node;
    } else {
        list->tail = new_node;
    }
}

void list_insert_before(List* list, List_Node* next_node, void* data) {
    xassert(next_node);
    List_Node* new_node = xmalloc(sizeof(List_Node));
    new_node->data = data;
    new_node->next = next_node;
    new_node->prev = next_node->prev;
    next_node->prev = new_node;
    if (new_node->prev) {
        new_node->prev->next = new_node;
    } else {
        list->head = new_node;
    }
}

// Bubble sorted atm. @Speed
void list_sort(List* l, bool (*func)(void*, void*)) {
    xassert(l && func);
    bool swapped = false;
    List_Node *a = NULL;
    do
    { 
        swapped = false; 
        a = l->head;
        while (a->next) 
        { 
            if ((*func)(a->data, a->next->data))
            {  
                void* tmp = a->data;
                a->data = a->next->data;
                a->next->data = tmp;

                swapped = true; 
            } 
            a = a->next; 
        } 
    } 
    while (swapped); 
}
