#pragma once
#include "utils.h"
#include <stddef.h>

typedef struct dlist {
    struct dlist *next,
               *prev;
    void *ptr;
    status (*free_ptr_func)(void *);
} dlist;

status init_node(dlist *node, void *content, status (*free_ptr_func)(void *));
status free_node(dlist *node);
status copy_node(dlist *dest, dlist *src);

status remove_node(dlist *node_to_remove);
status add_node(dlist *dest, dlist *src);

size_t size(dlist *node);
dlist *prev(dlist *node);
dlist *next(dlist *node);
dlist *end(dlist *node);
dlist *start(dlist *node);

status push(dlist *end, void *data);
status pull(dlist *end, void **data, int left);

