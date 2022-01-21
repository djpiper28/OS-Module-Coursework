#pragma once
#include "utils.h"
#include "dlist.h"
#include <stdio.h>

status free_dlist_node(void *node);
status init_dlist_node(dlist **node);

#define BLOCK_SIZE 5
#define MAX_NODE_INT 180

typedef struct virt_mem {
    size_t nodes, free_nodes;
    dlist *head,
          *tail,
          *left,
          *right;
} virt_mem;

status init_virt_mem(virt_mem *mem);
status free_virt_mem(virt_mem *mem);

status grow_virt_mem(virt_mem *mem, size_t amount);
status shrink_virt_mem(virt_mem *mem, size_t amount);

status add_int(virt_mem *mem, int i);
status remove_int(virt_mem *mem, int left);

#define print_virt_mem(mem, stream) print_virt_mem_conf(mem, stream)
void print_virt_mem_conf(virt_mem *mem, FILE *stream);
void print_virt_mem_non_conf(virt_mem *mem, FILE *stream);

