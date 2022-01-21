#include "dlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

status init_node(dlist *node, void *content, status (*free_func)(void *))
{
    memset(node, 0, sizeof * node);
    node->ptr = content;
    node->free_ptr_func = free_func;
    return SUCCESS;
}

status free_node(dlist *node)
{
    status ret;

    if (node == NULL) {
        return FAIL; /* Cannot free a null pointer */
    }

    if (node->next != NULL || node->prev != NULL) {
        return FAIL; /* Cannot free a used node. */
    }

    ret = node->free_ptr_func(node->ptr);
    node->ptr = NULL;

    return ret;
}

status copy_node(dlist *dest, dlist *src)
{
    if (dest == NULL || src == NULL) {
        return FAIL; /* Cannot copy to or from a null pointer */
    }

    memcpy(dest, src, sizeof * src);
    return SUCCESS;
}

status remove_node(dlist *node)
{
    dlist *prev, *next;

    if (node == NULL) {
        return FAIL; /* Cannot remove a null pointer */
    }

    prev = node->prev;
    next = node->next;

    /* Dereference the node */
    if (prev != NULL) {
        prev->next = next;
    }

    if (next != NULL) {
        next->prev = prev;
    }

    node->prev = node->next = NULL;

    /* Free the node */
    return free_node(node);
}

status add_node(dlist *dest, dlist *src)
{
    if (src == NULL || dest == NULL) {
        return FAIL; /* Cannot add to or from a null pointer */
    }

    src->next = dest->next;
    src->prev = dest;
    dest->next = src;

    if (src->next != NULL) {
        src->next->prev = src;
    }

    return SUCCESS;
}

size_t size(dlist *node)
{
    dlist *curr = node;
    status ret;
    if (node == NULL) {
        return 0;
    }

    ret = 0;
    for(; curr != NULL; curr = curr->next, ret++);

    return ret;
}

dlist *prev(dlist *node)
{
    if (node == NULL) {
        return NULL;
    }

    return node->prev;
}

dlist *next(dlist *node)
{
    if (node == NULL) {
        return NULL;
    }

    return node->next;
}

dlist *end(dlist *node)
{
    dlist *curr = node;

    if (curr == NULL) {
        return NULL;
    }

    for (; curr->next != NULL; curr = curr->next);

    return curr;
}

dlist *start(dlist *node)
{
    dlist *curr = node;

    if (curr == NULL) {
        return NULL;
    }

    for(; curr->prev != NULL; curr = curr->prev);

    return curr;
}

status push(dlist *end, void *data)
{
    dlist *p = end;
    if (p->next == NULL) {
        for (; p->ptr != NULL; p = prev(p)) {
            if (p->prev == NULL) {
                return FAIL;
            }
        }
    } else {
        for (; p->ptr != NULL; p = next(p)) {
            if (p->next == NULL) {
                return FAIL;
            }
        }
    }

    p->ptr = data;
    return SUCCESS;
}

status pull(dlist *end, void **data, int left)
{
    dlist *p = end;
    if (p->ptr == NULL) {
        fprintf(stderr, "Error: nothing to pull\n");
        return FAIL;
    }

    if (!left) {
        for (; p->ptr != NULL; p = prev(p)) {
            if (p->prev == NULL) {
                return FAIL;
            }
        }

        p = next(p);
    } else {
        for (; p->ptr != NULL; p = next(p)) {
            if (p->next == NULL) {
                return FAIL;
            }
        }

        p = prev(p);
    }

    if (p->ptr == NULL) {
        fprintf(stderr, "Error: there are no nodes to pull\n");
    }

    *data = p->ptr;
    p->ptr = NULL;
    return SUCCESS;
}

