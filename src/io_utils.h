#pragma once
#include "utils.h"
#include <stdio.h>

#define MAX 4096

typedef struct integer_array {
    size_t length;
    int *arr;
} integer_array;

status free_integer_array(integer_array *arr);
status init_integer_array(integer_array *arr, FILE *f);
status get_signed_int(int *ret, FILE *stream);

