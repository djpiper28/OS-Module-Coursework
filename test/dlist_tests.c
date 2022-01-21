#include "../src/dlist.h"
#include "dlist_tests.h"
#include "test_macros.h"
#include <string.h>

void *flag = NULL;
static status test_free(void *input)
{
    flag = input;
    return SUCCESS;
}

int test_init()
{
    void *ptr;
    dlist node;

    ptr = (void *) 5;
    ASSERT(init_node(&node, ptr, test_free) == SUCCESS);
    ASSERT(node.ptr == ptr);
    ASSERT(node.prev == NULL);
    ASSERT(node.next == NULL);
    ASSERT(node.free_ptr_func == test_free);

    return 1;
}

int test_init_free()
{
    void *ptr;
    dlist node;

    flag = NULL;
    ptr = (void *) 5;
    ASSERT(init_node(&node, ptr, test_free) == SUCCESS);
    ASSERT(free_node(&node) == SUCCESS);
    ASSERT(flag == ptr);

    ASSERT(free_node(NULL) == FAIL);

    /* Test error conditions and that memory is not changed */
    node.next = (dlist *) 1;
    ASSERT(free_node(&node) == FAIL);
    ASSERT(node.next == (dlist *) 1);

    node.prev = (dlist *) 1;
    ASSERT(free_node (&node) == FAIL);
    ASSERT(node.next == (dlist *) 1);
    ASSERT(node.prev == (dlist *) 1);

    node.next = NULL;
    ASSERT(free_node(&node) == FAIL);
    ASSERT(node.prev == (dlist *) 1);

    return 1;
}

int test_copy()
{
    void *ptr;
    dlist node, cpy;

    ptr = (void *) 5;
    ASSERT(init_node(&node, ptr, test_free) == SUCCESS);

    node.next = (dlist *) 2;
    node.prev = (dlist *) 3;

    /* Test copy */
    ASSERT(init_node(&node, ptr, test_free) == SUCCESS);
    ASSERT(copy_node(&cpy, &node) == SUCCESS);
    ASSERT(cpy.ptr == node.ptr);
    ASSERT(cpy.next == node.next);
    ASSERT(cpy.prev == node.prev);
    ASSERT(cpy.free_ptr_func == node.free_ptr_func);

    /* Test error conditions and that memory was not touched */
    ASSERT(copy_node(&cpy, NULL) == FAIL);
    ASSERT(cpy.ptr == node.ptr);
    ASSERT(cpy.next == node.next);
    ASSERT(cpy.prev == node.prev);
    ASSERT(cpy.free_ptr_func == node.free_ptr_func);

    ASSERT(copy_node(NULL, &node) == FAIL);
    ASSERT(cpy.ptr == node.ptr);
    ASSERT(cpy.next == node.next);
    ASSERT(cpy.prev == node.prev);
    ASSERT(cpy.free_ptr_func == node.free_ptr_func);

    return 1;
}

int test_add()
{
    dlist head, new, new_2, tail, head_b, head_c;

    /* Epic auto increment */
    size_t a = 0;
    void *head_ptr = (void *) a++;
    void *new_ptr = (void *) a++;
    void *new_2_ptr = (void *) a++;
    void *tail_ptr = (void *) a++;

    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new_2, new_2_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Test error conditions and, that the memory is not touched */
    head_b = head;
    ASSERT(add_node(&head, NULL) == FAIL);
    ASSERT(memcmp(&head, &head_b, sizeof(head)) == 0);

    head_c = head;
    ASSERT(add_node(NULL, &new) == FAIL);
    ASSERT(memcmp(&head, &head_c, sizeof(head)) == 0);

    /* Test add */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);
    ASSERT(add_node(&new, &new_2) == SUCCESS);
    ASSERT(head.next == &new);
    ASSERT(head.prev == NULL);

    ASSERT(new.prev == &head);
    ASSERT(new.next == &new_2);

    ASSERT(new_2.prev == &new);
    ASSERT(new_2.next == &tail);

    ASSERT(tail.prev == &new_2);
    ASSERT(tail.next == NULL);

    return 1;
}

int test_remove()
{
    dlist head, new, tail;

    /* Epic auto increment */
    size_t a = 0;
    void *head_ptr = (void *) a++;
    void *new_ptr = (void *) a++;
    void *tail_ptr = (void *) a++;

    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    /* Test error conditions and that memory was not touched */
    ASSERT(remove_node(NULL) == FAIL);

    /* Test remove the tail */
    flag = NULL;

    ASSERT(remove_node(&tail) == SUCCESS);
    ASSERT(flag == tail_ptr);
    ASSERT(new.next == NULL);

    ASSERT(tail.next == NULL);
    ASSERT(tail.prev == NULL);

    /* Test remove in the middle */
    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    ASSERT(remove_node(&new) == SUCCESS);
    ASSERT(head.prev == NULL);
    ASSERT(head.next == &tail);

    ASSERT(tail.prev == &head);
    ASSERT(tail.next == NULL);

    ASSERT(new.next == NULL);
    ASSERT(new.prev == NULL);

    /* Test remove head */
    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    ASSERT(remove_node(&head) == SUCCESS);
    ASSERT(head.prev == NULL);
    ASSERT(head.next == NULL);

    ASSERT(tail.prev == &new);
    ASSERT(tail.next == NULL);

    ASSERT(new.next == &tail);
    ASSERT(new.prev == NULL);

    return 1;
}

int test_add_remove()
{
    dlist head, new, tail;

    /* Epic auto increment */
    size_t a = 0;
    void *head_ptr = (void *) a++;
    void *new_ptr = (void *) a++;
    void *tail_ptr = (void *) a++;

    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    /* Test removal then readdition (after reinit) */
    ASSERT(remove_node(&tail) == SUCCESS);

    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS)
    ASSERT(add_node(&new, &tail) == SUCCESS);

    ASSERT(remove_node(&tail) == SUCCESS);
    ASSERT(remove_node(&tail) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS)

    ASSERT(add_node(&new, &tail) == SUCCESS);

    /* Test state */
    ASSERT(head.next == &new);
    ASSERT(head.prev == NULL);

    ASSERT(new.prev == &head);
    ASSERT(new.next == &tail);

    ASSERT(tail.prev == &new);
    ASSERT(tail.next == NULL);

    return 1;
}

int test_next()
{
    dlist head, new, tail;

    /* Epic auto increment */
    size_t a = 0;
    void *head_ptr = (void *) a++;
    void *new_ptr = (void *) a++;
    void *tail_ptr = (void *) a++;

    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    /* Test next */
    ASSERT(next(&head) == &new);
    ASSERT(next(next(&head)) == &tail);

    ASSERT(next(NULL) == NULL);
    ASSERT(next(&tail) == NULL);

    return 1;
}

int test_prev()
{
    dlist head, new, tail;

    /* Epic auto increment */
    size_t a = 0;
    void *head_ptr = (void *) a++;
    void *new_ptr = (void *) a++;
    void *tail_ptr = (void *) a++;

    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    /* Test prev */
    ASSERT(prev(&new) == &head);
    ASSERT(prev(prev(&tail)) == &head);

    ASSERT(prev(NULL) == NULL);
    ASSERT(prev(&head) == NULL);

    return 1;
}

int test_next_prev()
{
    dlist head, new, tail;

    /* Epic auto increment */
    size_t a = 0;
    void *head_ptr = (void *) a++;
    void *new_ptr = (void *) a++;
    void *tail_ptr = (void *) a++;

    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    /* Test prev */
    ASSERT(prev(&new) == &head);
    ASSERT(prev(prev(&tail)) == &head);

    ASSERT(prev(NULL) == NULL);
    ASSERT(prev(&head) == NULL);

    ASSERT(next(prev(&head)) == NULL);
    ASSERT(next(prev(&new)) == &new);
    ASSERT(next(prev(next(&head))) == &new);

    return 1;
}

int test_size()
{
    dlist head, new, tail;

    /* Epic auto increment */
    size_t a = 0;
    void *head_ptr = (void *) a++;
    void *new_ptr = (void *) a++;
    void *tail_ptr = (void *) a++;

    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(size(NULL) == 0);

    ASSERT(size(&head) == 1);
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(size(&head) == 2);
    ASSERT(add_node(&new, &tail) == SUCCESS);
    ASSERT(size(&head) == 3);

    return 1;
}

int test_start()
{
    dlist head, new, tail;

    /* Epic auto increment */
    size_t a = 0;
    void *head_ptr = (void *) a++;
    void *new_ptr = (void *) a++;
    void *tail_ptr = (void *) a++;

    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    /* Test start */
    ASSERT(start(&tail) == &head);
    ASSERT(start(&head) == &head);

    ASSERT(start(NULL) == NULL);

    return 1;
}

int test_end()
{
    dlist head, new, tail;

    /* Epic auto increment */
    size_t a = 0;
    void *head_ptr = (void *) a++;
    void *new_ptr = (void *) a++;
    void *tail_ptr = (void *) a++;

    ASSERT(init_node(&head, head_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, new_ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, tail_ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    /* Test end */
    ASSERT(end(&tail) == &tail);
    ASSERT(end(&head) == &tail);

    ASSERT(end(NULL) == NULL);

    return 1;
}

int test_push()
{
    dlist head, new, tail;
    void *data_in;

    /* Epic auto increment */
    void *ptr = NULL;

    ASSERT(init_node(&head, ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, ptr, test_free) == SUCCESS);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    data_in = (void *) 5;
    ASSERT(push(&head, data_in) == SUCCESS);
    ASSERT(data_in == head.ptr);

    ASSERT(push(&tail, data_in) == SUCCESS);
    ASSERT(data_in == tail.ptr);

    data_in = (void *) 4;
    ASSERT(push(&tail, data_in) == SUCCESS);
    ASSERT(data_in == new.ptr);

    ASSERT(push(&head, data_in) == FAIL);
    ASSERT(push(&tail, data_in) == FAIL);

    /* Check that the memory was not touched */
    ASSERT(head.next == &new);
    ASSERT(head.prev == NULL);

    ASSERT(new.prev == &head);
    ASSERT(new.next == &tail);

    ASSERT(tail.prev == &new);
    ASSERT(tail.next == NULL);

    return 1;
}

int test_pull()
{
    dlist head, new, tail, dummy;
    void *data_in, *data_out;

    /* Epic auto increment */
    void *ptr = NULL;

    ASSERT(init_node(&head, ptr, test_free) == SUCCESS);
    ASSERT(init_node(&new, ptr, test_free) == SUCCESS);
    ASSERT(init_node(&tail, ptr, test_free) == SUCCESS);
    ASSERT(init_node(&dummy, (void *) 5, test_free) == SUCCESS);

    ASSERT(pull(&dummy, &data_out, 0) == FAIL);
    ASSERT(pull(&dummy, &data_out, 1) == FAIL);

    /* Add nodes */
    ASSERT(add_node(&head, &new) == SUCCESS);
    ASSERT(add_node(&new, &tail) == SUCCESS);

    data_in = (void *) 5;
    ASSERT(push(&head, data_in) == SUCCESS);
    ASSERT(push(&tail, data_in) == SUCCESS);

    ASSERT(pull(&tail, &data_out, 0) == SUCCESS);
    ASSERT(data_in == data_out);
    ASSERT(tail.ptr == NULL);

    ASSERT(pull(&head, &data_out, 1) == SUCCESS);
    ASSERT(data_in == data_out);
    ASSERT(head.ptr == NULL);

    /* Test line 165 */
    ASSERT(push(&tail, data_in) == SUCCESS);
    ASSERT(push(&tail, data_in) == SUCCESS);

    ASSERT(pull(&tail, &data_out, 0) == SUCCESS);
    ASSERT(data_in == data_out);
    ASSERT(pull(&tail, &data_out, 0) == SUCCESS);
    ASSERT(data_in == data_out);

    /* Test line 172 */
    ASSERT(push(&head, data_in) == SUCCESS);
    ASSERT(push(&head, data_in) == SUCCESS);
    ASSERT(push(&head, data_in) == SUCCESS);

    ASSERT(pull(&head, &data_out, 1) == FAIL);
    head.ptr = new.ptr = tail.ptr = ptr;

    /* Test error conditions*/
    ASSERT(pull(&head, &data_out, 1) == FAIL);
    ASSERT(pull(&tail, &data_out, 0) == FAIL);

    ASSERT(push(&tail, data_in) == SUCCESS);
    ASSERT(push(&tail, data_in) == SUCCESS);
    ASSERT(push(&tail, data_in) == SUCCESS);
    ASSERT(pull(&tail, &data_out, 0) == FAIL);

    return 1;
}
