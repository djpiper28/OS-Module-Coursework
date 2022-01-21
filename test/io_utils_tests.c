#include "io_utils_tests.h"
#include "test_macros.h"
#include "../src/io_utils.h"
#include "../src/dlist.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define INT_ARR_TEST "+0 +1 +2 +3 +4"
#define LEN 5
#define INT_ARR_FAIL_TEST "+5 + 5 +6 66 ----- fail"

int test_integer_array()
{
    int fid[2];
    integer_array arr;
    FILE *r, *w;

    /* test success cases */
    ASSERT(pipe(fid) == 0);
    r = fdopen(fid[0], "r");
    w = fdopen(fid[1], "w");

    ASSERT(r != NULL);
    ASSERT(w != NULL);

    ASSERT(fprintf(w, INT_ARR_TEST) == strlen(INT_ARR_TEST));
    ASSERT(fflush(w) == 0);
    fclose(w);

    ASSERT(init_integer_array(&arr, r) == SUCCESS);
    ASSERT(arr.length == LEN);
    for (int i = 0; i < LEN; i++) {
        ASSERT(arr.arr[i] == i);
    }

    fclose(r);
    ASSERT(free_integer_array(&arr) == SUCCESS);

    /* test fail case */
    memset(&arr, 0, sizeof(arr));

    ASSERT(pipe(fid) == 0);
    r = fdopen(fid[0], "r");
    w = fdopen(fid[1], "w");

    ASSERT(r != NULL);
    ASSERT(w != NULL);

    ASSERT(fprintf(w, INT_ARR_FAIL_TEST) == strlen(INT_ARR_FAIL_TEST));
    ASSERT(fflush(w) == 0);
    fclose(w);

    ASSERT(init_integer_array(&arr, r) == FAIL);
    ASSERT(arr.length == 0);

    fclose(r);
    ASSERT(free_integer_array(&arr) == FAIL);
    return 1;
}

#define F_CONTENT "plus + 2  3 - 3four4 +43\n"\
				          "+9999+69++23+1"

int test_get_signed_int()
{
    int res, fid[2];
    FILE *r, *w;
    ASSERT(pipe(fid) == 0);
    r = fdopen(fid[0], "r");
    w = fdopen(fid[1], "w");

    ASSERT(r != NULL);
    ASSERT(w != NULL);
    ASSERT(fprintf(w, F_CONTENT) == strlen(F_CONTENT));
    ASSERT(fflush(w) == 0);
    fclose(w);

    ASSERT(get_signed_int(&res, r) == SUCCESS);
    ASSERT(res == 23);

    ASSERT(get_signed_int(&res, r) == SUCCESS);
    ASSERT(res == -34);

    ASSERT(get_signed_int(&res, r) == SUCCESS);
    ASSERT(res == 43);

    ASSERT(get_signed_int(&res, r) == FAIL); /* Test that big numbers trigger errors */
    ASSERT(get_signed_int(&res, r) == SUCCESS);
    ASSERT(res == 69);

    ASSERT(get_signed_int(&res, r) == FAIL); /* Test that duplicate signs trigger errors */
    ASSERT(get_signed_int(&res, r) == FAIL); /* Test that unsigned numbers fails */
    while (get_signed_int(&res, r) != EOF_FAIL); /* Test for EOF_FAIL when the buffer for the read is empty */

    fclose(r);
    return 1;
}
