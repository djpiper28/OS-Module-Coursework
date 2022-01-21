#include "virt_mem_tests.h"
#include "test_macros.h"
#include "../src/virt_mem.h"
#include "../src/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int test_init_free_virt_mem()
{
    virt_mem mem;

    ASSERT(init_virt_mem(&mem) == SUCCESS);

    ASSERT(end(mem.left) == mem.right);
    ASSERT(mem.left == start(mem.right));

    ASSERT(mem.head == mem.left);
    ASSERT(mem.left != NULL);

    ASSERT(mem.tail == mem.right);
    ASSERT(mem.right != NULL);

    ASSERT(mem.nodes == BLOCK_SIZE);
    ASSERT(mem.free_nodes == mem.nodes);
    ASSERT(free_virt_mem(&mem) == SUCCESS);

    return 1;
}

int test_grow_virt_mem()
{
    virt_mem mem;
    int i;

    ASSERT(init_virt_mem(&mem) == SUCCESS);

    for (i = 2; i < 2 + 10; i++) {
        ASSERT(grow_virt_mem(&mem, BLOCK_SIZE) == SUCCESS);
        ASSERT(mem.left != NULL);
        ASSERT(mem.right != NULL);

        ASSERT(prev(mem.right) != NULL);
        ASSERT(mem.nodes == i * BLOCK_SIZE);
    }

    ASSERT(end(mem.left) == mem.right);
    ASSERT(mem.left == start(mem.right));

    ASSERT(end(mem.head) == mem.tail);
    ASSERT(start(mem.tail) == mem.head);

    ASSERT(free_virt_mem(&mem) == SUCCESS);

    return 1;
}

int test_shrink_virt_mem()
{
    virt_mem mem;
    size_t init_length;

    ASSERT(init_virt_mem(&mem) == SUCCESS);
    init_length = mem.nodes;

    ASSERT(shrink_virt_mem(&mem, BLOCK_SIZE / 2) == SUCCESS);
    ASSERT(mem.nodes = init_length - (BLOCK_SIZE / 2));

    ASSERT(grow_virt_mem(&mem, BLOCK_SIZE) == SUCCESS);
    ASSERT(shrink_virt_mem(&mem, BLOCK_SIZE) == SUCCESS);

    ASSERT(shrink_virt_mem(&mem, mem.nodes) == FAIL);
    ASSERT(shrink_virt_mem(&mem, mem.free_nodes) == FAIL);

    ASSERT(free_virt_mem(&mem) == SUCCESS);

    return 1;
}

int test_add_int()
{
    int i;
    virt_mem mem;

    ASSERT(init_virt_mem(&mem) == SUCCESS);

    for (i = 0; i < BLOCK_SIZE * 2; i++) {
        ASSERT(add_int(&mem, 1) == SUCCESS);
        ASSERT(mem.right != NULL);
        ASSERT(mem.left != NULL);
    }

    for (i = 0; i < BLOCK_SIZE * 2; i++) {
        ASSERT(add_int(&mem, 2) == SUCCESS);
        ASSERT(mem.left != NULL);
        ASSERT(mem.right != NULL);
    }

    ASSERT(free_virt_mem(&mem) == SUCCESS);

    return 1;
}

int test_remove_int()
{
    int i;
    virt_mem mem;

    ASSERT(init_virt_mem(&mem) == SUCCESS);

    ASSERT(remove_int(&mem, 1) == FAIL);
    ASSERT(remove_int(&mem, 0) == FAIL);

    for (i = 0; i < BLOCK_SIZE * 2; i++) {
        ASSERT(add_int(&mem, 1) == SUCCESS);
    }

    ASSERT(mem.nodes == BLOCK_SIZE * 3);
    ASSERT(mem.left != mem.right);
    ASSERT(remove_int(&mem, 0) == FAIL);

    ASSERT(end(mem.head) == mem.tail);
    ASSERT(start(mem.tail) == mem.head);

    for (i = 0; i < BLOCK_SIZE * 2; i++) {
        ASSERT(remove_int(&mem, 1) == SUCCESS);

        ASSERT(end(mem.head) == mem.tail);
        ASSERT(start(mem.tail) == mem.head);
    }

    ASSERT(mem.nodes = BLOCK_SIZE);

    ASSERT(free_virt_mem(&mem) == SUCCESS);

    return 1;
}

#define BIG_TEST_BUFFER_LENGTH (BLOCK_SIZE * 10)
#define EMPTY_VIRT_MEM_STR "| | | | | |\n"
#define EMPTY_R_VIRT_MEM_STR "| 0 | -1 | -1 | -1 | -1 | -1 | 0 |\n"
/* the list will grow as there would be one remaining node after insertion */
#define VIRT_MEM_STR "| 1 | 3 | | | | | | | 4 | 2 |\n"

int test_print_virt_mem()
{
    virt_mem mem;
    int fid[2], i;
    FILE *r, *w;
    char buffer[BIG_TEST_BUFFER_LENGTH];

    ASSERT(init_virt_mem(&mem) == SUCCESS);
    ASSERT(BLOCK_SIZE == 5); /* tests require this length as I said so. */

    ASSERT(pipe(fid) == 0);
    ASSERT((r = fdopen(fid[0], "r")) != NULL);
    ASSERT((w = fdopen(fid[1], "w")) != NULL);

    print_virt_mem_non_conf(&mem, w);
    ASSERT(fflush(w) == 0);
    ASSERT(fgets(buffer, BIG_TEST_BUFFER_LENGTH, r) == buffer);
    ASSERT(strcmp(buffer, EMPTY_VIRT_MEM_STR) == 0);

    print_virt_mem(&mem, w);
    ASSERT(fflush(w) == 0);
    ASSERT(fgets(buffer, BIG_TEST_BUFFER_LENGTH, r) == buffer);
    ASSERT(strcmp(buffer, EMPTY_R_VIRT_MEM_STR) == 0);

    for (i = 1; i < 5; i++) {
        ASSERT(add_int(&mem, i) == SUCCESS);
    }

    print_virt_mem_non_conf(&mem, w);
    ASSERT(fflush(w) == 0);
    ASSERT(fgets(buffer, BIG_TEST_BUFFER_LENGTH, r) == buffer);
    ASSERT(strcmp(buffer, VIRT_MEM_STR) == 0);

    ASSERT(fclose(w) == 0);
    ASSERT(fclose(r) == 0);

    ASSERT(free_virt_mem(&mem) == SUCCESS);
    return 1;
}

