#include "virt_mem.h"
#include <stdlib.h>
#include <stdio.h>

status free_dlist_node(void *content)
{
    if (content != NULL) {
        free(content);
    }
    return SUCCESS;
}

status init_dlist_node(dlist **node)
{
    *node = malloc(sizeof * * node);
    if (*node == NULL) {
        fprintf(stderr, "Error: failed to malloc dlist for new node\n");
        return FAIL;
    }

    init_node(*node, NULL, free_dlist_node);
    return SUCCESS;
}

status init_virt_mem(virt_mem *mem)
{
    int i;
    dlist *tmp, *last;
    status ret = SUCCESS, ret_inner = SUCCESS;

    mem->nodes = mem->free_nodes = BLOCK_SIZE;
    mem->head = mem->tail = mem->left = mem->right = NULL;

    for (i = 0; ret == SUCCESS && i < BLOCK_SIZE; i++) {
        ret = init_dlist_node(&tmp);
        if (ret) {
            if (i == 0) {
                mem->head = mem->left = tmp;
            } else {
                ret = add_node(last, tmp);
            }

            last = tmp;
        }
    }

    // On malloc fail free malloced nodes
    if (ret == FAIL) {
        while (ret_inner == SUCCESS) {
            tmp = next(mem->head);
            ret_inner = remove_node(mem->head);
            free(mem->head);

            if (ret_inner == FAIL) {
                fprintf(stderr, "Error: failed to remove node after initialisation error\n");
            }

            // Break from loop when there are no more nodes to free
            mem->head = tmp;
            if (mem->head == NULL) {
                break;
            }
        }
    } else {
        mem->tail = mem->right = last;
    }

    return ret;
}

status free_virt_mem(virt_mem *mem)
{
    status ret = SUCCESS;
    dlist *tmp;
    while (ret == SUCCESS) {
        tmp = next(mem->head);
        ret = remove_node(mem->head);
        free(mem->head);

        if (ret == FAIL) {
            fprintf(stderr, "Error: failed to free node of virtual memory\n");
        }

        // Break from loop when there are no more nodes to free
        mem->head = tmp;
        if (mem->head == NULL) {
            break;
        }
    }

    return ret;
}

status grow_virt_mem(virt_mem *mem, size_t amount)
{
    size_t i;
    dlist *tmp;
    status ret = SUCCESS;

    if (amount + mem->nodes <= mem->nodes) {
        fprintf(stderr, "Error: adding additional blocks causes an overflow error\n");
        return FAIL;
    }

    for (i = 0; ret == SUCCESS && i < amount; i++) {
        ret = init_dlist_node(&tmp);
        if (ret) {
            ret = add_node(mem->left, tmp);
            mem->nodes++;
            mem->free_nodes++;
        }
    }

    if (ret == FAIL) {
        fprintf(stderr, "Error: failed to grow virtual memory\n");
    }

    return ret;
}

status shrink_virt_mem(virt_mem *mem, size_t amount)
{
    status ret = SUCCESS;
    dlist *current, *tmp;
    size_t i = 0;

    if (amount >= mem->free_nodes - 1) {
        return FAIL; /* Shrinking this will cause data to be removed. */
    }

    current = next(mem->left);
    for (i = 0; i < amount && ret == SUCCESS; i++) {
        if (current == NULL) {
            fprintf(stderr, "Error: unable to free more nodes during shrink\n");
            return FAIL;
        }

        tmp = current;
        current = next(current);

        ret = remove_node(tmp);
        if (ret == SUCCESS) {
            mem->nodes--;
            mem->free_nodes--;
            free(tmp);
        } else {
            fprintf(stderr, "Error: unable to free nodes during shrink\n");
        }
    }

    return ret;
}

status add_int(virt_mem *mem, int i)
{
    status ret;
    dlist *node;
    int *num;

    num = malloc(sizeof * num);
    if (num == NULL) {
        fprintf(stderr, "Error: malloc failed trying to init integer for virtual memory\n");
        return FAIL;
    }
    *num = i;

    if (mem->free_nodes <= 2) {
        ret = grow_virt_mem(mem, BLOCK_SIZE);
        if (ret == FAIL) {
            free(num);
            fprintf(stderr, "Error: failed to grow virtual memory during add\n");
            return FAIL;
        }
    }

    if (i & 1) { /* is odd */
        node = mem->left;
    } else {
        node = mem->right;
    }

    ret = push(node, num);

    if (ret == SUCCESS) {
        if (i & 1) { /* is odd */
            mem->left = next(mem->left);
        } else {
            mem->right = prev(mem->right);
        }

        mem->free_nodes--;
    } else {
        free(num);
        fprintf(stderr, "Error: failed to push to virtual memory\n");
    }

    if (mem->right == NULL) {
        mem->right = mem->tail;
    }
    return ret;
}

status remove_int(virt_mem *mem, int left)
{
    void *output;
    dlist *node;
    status ret;

    if (left) {
        node = prev(mem->left);
    } else {
        node = next(mem->right);
    }

    if (node == NULL) {
        return FAIL;
    }

    ret = pull(node, &output, left);
    if (ret == FAIL) {
        return FAIL;
    }

    ret = SUCCESS;
    mem->free_nodes++;

    free(output);

    if (left) {
        mem->left = prev(mem->left);
        if (mem->left == NULL) {
            mem->left = mem->head;
        }
    } else {
        mem->right = next(mem->right);
        if (mem->right == NULL) {
            mem->right = mem->tail;
        }
    }

    if (mem->free_nodes > BLOCK_SIZE + 1) {
        ret = shrink_virt_mem(mem, BLOCK_SIZE);
    }

    return ret;
}

void print_virt_mem_conf(virt_mem *mem, FILE *stream)
{
    dlist *curr;
    fprintf(stream, "| 0 ");
    for (curr = mem->head; curr != NULL; curr = next(curr)) {
        if (curr->ptr == NULL) {
            fprintf(stream, "| -1 ");
        } else {
            fprintf(stream, "| %d ", *(int*) curr->ptr);
        }
    }
    fprintf(stream, "| 0 |\n");
}

void print_virt_mem_non_conf(virt_mem *mem, FILE *stream)
{
    dlist *curr;
    for (curr = mem->head; curr != NULL; curr = next(curr)) {
        if (curr->ptr == NULL) {
            fprintf(stream, "| ");
        } else {
            fprintf(stream, "| %d ", *(int*) curr->ptr);
        }
    }
    fprintf(stream, "|\n");
}

