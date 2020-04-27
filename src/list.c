typedef struct List_Node
{
    void* data;
    struct List_Node* next;
    struct List_Node* prev;
} List_Node;

typedef struct
{
    List_Node* head;
    List_Node* tail;
    s64 count;
} List;

#define list_foreach_reverse(list) for (List_Node* it = list->tail; it; it = it->prev)
#define list_foreach(list) for (List_Node* it = list->head; it; it = it->next)

INTERNAL List* make_list(void);
INTERNAL void list_free(List* l);
INTERNAL void list_sort(List* l, bool (*func)(void*, void*));
INTERNAL void* list_append(List* list, void* data);
INTERNAL void* list_prepend(List* list, void* data);
INTERNAL void* list_last(List* list);
INTERNAL void* list_first(List* list);
INTERNAL void* list_at(List* list, s64 index);
INTERNAL void list_insert_after(List* list, List_Node* prev_node, void* data);
INTERNAL void list_insert_before(List* list, List_Node* next_node, void* data);
INTERNAL void list_append_content_of(List* list, List* other_list);
INTERNAL void list_append_content_of_in_reverse(List* list, List* other_list);
INTERNAL void list_prepend_content_of(List* list, List* other_list);
INTERNAL void list_prepend_content_of_in_reverse(List* list, List* other_list);
INTERNAL void* list_remove_at(List* list, s64 index);
INTERNAL void* list_remove(List* list, List_Node* node);
INTERNAL bool list_empty(List* list);

INTERNAL List*
make_list()
{
    List* l = xmalloc(sizeof(List));
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
    return l;
}

INTERNAL void
list_init(List* l)
{
    l = xmalloc(sizeof(List));
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
}

INTERNAL void
list_free(List* l)
{
    list_foreach(l)
    {
        free(it->prev);
    }
}

INTERNAL bool
list_empty(List* list)
{
    return (list->head == NULL && list->tail == NULL);
}

INTERNAL void list_prepend_content_of_in_reverse(List* list, List* other_list)
{
    xassert(list);
    xassert(other_list);
    list_foreach_reverse(other_list) {
        list_prepend(list, it->data);
    }
}
INTERNAL void list_prepend_content_of(List* list, List* other_list)
{
    xassert(list);
    xassert(other_list);
    list_foreach(other_list) {
        list_prepend(list, it->data);
    }
}

INTERNAL void list_append_content_of_in_reverse(List* list, List* other_list)
{
    xassert(list);
    xassert(other_list);
    list_foreach_reverse(other_list) {
        list_append(list, it->data);
    }
}

INTERNAL void list_append_content_of(List* list, List* other_list)
{
    xassert(list);
    xassert(other_list);
    list_foreach(other_list)
    {
        list_append(list, it->data);
    }
}

INTERNAL void* list_remove_at(List* list, s64 index)
{
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

INTERNAL void* list_remove_at_end(List* list)
{
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

INTERNAL void* list_remove_at_start(List* list) {
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

INTERNAL void* list_remove(List* list, List_Node* node) {
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

INTERNAL void* list_at(List* list, s64 index) {
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

INTERNAL void* list_prepend(List* list, void* data) {
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

INTERNAL void* list_append(List* list, void* data) {
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

INTERNAL void* list_first(List* list) {
    xassert(list);
    return list->head->data;
}

INTERNAL void* list_last(List* list) {
    xassert(list);
    return list->tail->data;
}

INTERNAL void list_insert_after(List* list, List_Node* prev_node, void* data) {
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

INTERNAL void list_insert_before(List* list, List_Node* next_node, void* data) {
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
INTERNAL void list_sort(List* l, bool (*func)(void*, void*)) {
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
