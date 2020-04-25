typedef struct list_node_t
{
    void* data;
    struct list_node_t* next;
    struct list_node_t* prev;
} list_node_t;

typedef struct
{
    list_node_t* head;
    list_node_t* tail;
    s64 count;
} list_t;

#define list_foreach_reverse(list) for (list_node_t* it = list->tail; it; it = it->prev)
#define list_foreach(list) for (list_node_t* it = list->head; it; it = it->next)

internal list_t* make_list(void);
internal void list_free(list_t* l);
internal void list_sort(list_t* l, bool (*func)(void*, void*));
internal void* list_append(list_t* list, void* data);
internal void* list_prepend(list_t* list, void* data);
internal void* list_last(list_t* list);
internal void* list_first(list_t* list);
internal void* list_at(list_t* list, s64 index);
internal void list_insert_after(list_t* list, list_node_t* prev_node, void* data);
internal void list_insert_before(list_t* list, list_node_t* next_node, void* data);
internal void list_append_content_of(list_t* list, list_t* other_list);
internal void list_append_content_of_in_reverse(list_t* list, list_t* other_list);
internal void list_prepend_content_of(list_t* list, list_t* other_list);
internal void list_prepend_content_of_in_reverse(list_t* list, list_t* other_list);
internal void* list_remove_at(list_t* list, s64 index);
internal void* list_remove(list_t* list, list_node_t* node);
internal bool list_empty(list_t* list);

internal list_t*
make_list()
{
    list_t* l = xmalloc(sizeof(list_t));
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
    return l;
}

internal void
list_init(list_t* l)
{
    l = xmalloc(sizeof(list_t));
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
}

internal void
list_free(list_t* l)
{
    list_foreach(l)
    {
        free(it->prev);
    }
}

internal bool
list_empty(list_t* list)
{
    return (list->head == NULL && list->tail == NULL);
}

internal void list_prepend_content_of_in_reverse(list_t* list, list_t* other_list)
{
    xassert(list);
    xassert(other_list);
    list_foreach_reverse(other_list) {
        list_prepend(list, it->data);
    }
}
internal void list_prepend_content_of(list_t* list, list_t* other_list)
{
    xassert(list);
    xassert(other_list);
    list_foreach(other_list) {
        list_prepend(list, it->data);
    }
}

internal void list_append_content_of_in_reverse(list_t* list, list_t* other_list)
{
    xassert(list);
    xassert(other_list);
    list_foreach_reverse(other_list) {
        list_append(list, it->data);
    }
}

internal void list_append_content_of(list_t* list, list_t* other_list)
{
    xassert(list);
    xassert(other_list);
    list_foreach(other_list)
    {
        list_append(list, it->data);
    }
}

internal void* list_remove_at(list_t* list, s64 index)
{
    xassert(list);
    xassert(index >= 0 && index <= list->count);

    bool start_from_tail = (list->count - index) < index ? true : false;

    list_node_t* removed_node = NULL;
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

internal void* list_remove_at_end(list_t* list)
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

internal void* list_remove_at_start(list_t* list) {
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

internal void* list_remove(list_t* list, list_node_t* node) {
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
        list_node_t* current = list->head;
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

internal void* list_at(list_t* list, s64 index) {
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

internal void* list_prepend(list_t* list, void* data) {
    xassert(list);
    // xassert(data);
    if (!data) return data;
    ++list->count;
    list_node_t* new_node = xmalloc(sizeof(list_node_t));
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

internal void* list_append(list_t* list, void* data) {
    tassert(list && data, "%zu, %zu", list, data);
    // xassert(data);
    if (!data) return data;
    ++list->count;
    list_node_t* new_node = xmalloc(sizeof(list_node_t));
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

internal void* list_first(list_t* list) {
    xassert(list);
    return list->head->data;
}

internal void* list_last(list_t* list) {
    xassert(list);
    return list->tail->data;
}

internal void list_insert_after(list_t* list, list_node_t* prev_node, void* data) {
    xassert(prev_node);
    list_node_t* new_node = xmalloc(sizeof(list_node_t));
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

internal void list_insert_before(list_t* list, list_node_t* next_node, void* data) {
    xassert(next_node);
    list_node_t* new_node = xmalloc(sizeof(list_node_t));
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
internal void list_sort(list_t* l, bool (*func)(void*, void*)) {
    xassert(l && func);
    bool swapped = false;
    list_node_t *a = NULL;
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
