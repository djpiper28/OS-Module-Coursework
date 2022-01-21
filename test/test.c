#include <stdio.h>
#include <stdlib.h>
#include "dlist_tests.h"
#include "io_utils_tests.h"
#include "virt_mem_tests.h"

int test_func(int (*test_method)(), const char *test_name)
{
    int status;

    printf("Info: Testing %s:\n", test_name);

    status = test_method();
    if (status) {
        printf("Test-Pass: Test %s passed.\n", test_name);
    } else {
        printf("Test-Fail: Test %s failed.\n", test_name);
    }

    return status;
}

typedef struct unit_test {
    int (*test_method)();
    const char *test_name;
} unit_test;

int test_dlist()
{
    int failed, len, status, i;

    unit_test tests[] = {
        {test_init, "init"},
        {test_init_free, "init and, free (various permutations)"},
        {test_copy, "copy"},
        {test_add, "add_node"},
        {test_remove, "remove node"},
        {test_add_remove, "add and, remove (various permutations)"},
        {test_next, "next"},
        {test_prev, "prev"},
        {test_next_prev, "next and, prev (various permutations)"},
        {test_size, "size"},
        {test_start, "start"},
        {test_end, "end"},
        {test_push, "push"},
        {test_pull, "pull"}
    };

    failed = 0;
    len = (int) sizeof(tests) / sizeof(unit_test);
    printf("Info: Running %d tests for dlist.h\n", len);
    for (i = 0; i < len; i++) {
        unit_test test = tests[i];
        status = test_func(test.test_method, test.test_name);

        if (!status) {
            failed++;
        }
    }

    return failed;
}

int test_io_utils()
{
    int failed, len, status, i;

    unit_test tests[] = {
        {test_integer_array, "init_integer_array and, free_integer_array (various cases)"},
        {test_get_signed_int, "get_signed_int"}
    };

    failed = 0;
    len = (int) sizeof(tests) / sizeof(unit_test);
    printf("Info: Running %d tests for io_utils.h\n", len);
    for (i = 0; i < len; i++) {
        unit_test test = tests[i];
        status = test_func(test.test_method, test.test_name);

        if (!status) {
            failed++;
        }
    }

    return failed;
}

int test_virt_mem()
{
    int failed, len, status, i;

    unit_test tests[] = {
        {test_init_free_virt_mem, "init_virt_mem and, free_virt_mem (various permutations)"},
        {test_grow_virt_mem, "grow_virt_mem"},
        {test_shrink_virt_mem, "shrink_virt_mem"},
        {test_add_int, "add_int"},
        {test_remove_int, "remove_int"},
        {test_print_virt_mem, "print_virt_mem"}
    };

    failed = 0;
    len = (int) sizeof(tests) / sizeof(unit_test);
    printf("Info: Running %d tests for io_utils.h\n", len);
    for (i = 0; i < len; i++) {
        unit_test test = tests[i];
        status = test_func(test.test_method, test.test_name);

        if (!status) {
            failed++;
        }
    }

    return failed;
}

int main(int argc, char **argv)
{
    int status;

    if (argc > 1) {
        printf("Info: %d command line arguments were ignored.\n", argc - 1);
    }

    printf("Info: Running tests for Danny's Doubly Linked Lists coursework.\n");
    printf("Info: Running tests for dlist.h\n");

    status = test_dlist();

    printf("Info: Running tests for io_utils.h\n");
    status += test_io_utils();

    printf("Info: Running tests for virt_mem.h\n");
    status += test_virt_mem();

    if (status != 0) {
        printf("Warn: %d tests failed, see output above.\n", status);
    }

    return status;
}

