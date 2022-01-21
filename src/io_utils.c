#include "io_utils.h"
#include "dlist.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

status free_integer_array(integer_array *arr)
{
    if (arr->arr == NULL) {
        return FAIL;
    }

    free(arr->arr);
    arr->arr = NULL;
    arr->length = 0;
    return SUCCESS;
}

status init_integer_array(integer_array *arr, FILE *f)
{
    int fid[2], i, *buffer, *tmp_buffer, tmp, read_input;
    FILE *r, *w;
    char buffer_read[BUFFER_LENGTH];
    status read_status;

    arr->length = 0;
    arr->arr = NULL;

    if (pipe(fid) != 0) {
        return FAIL;
    }

    r = fdopen(fid[0], "r");
    w = fdopen(fid[1], "w");

    if (r == NULL || w == NULL) {
        if (r != NULL) fclose(r);
        if (w != NULL) fclose(w);
        return FAIL;
    }

    if (fgets(buffer_read, BUFFER_LENGTH, f) == NULL) {
        fclose(r);
        fclose(w);
        return EOF_FAIL;
    }

    if (fprintf(w, "%s", buffer_read) != strlen(buffer_read)) {
        fclose(w);
        fclose(r);
        return FAIL;
    }

    fflush(w);
    fclose(w);

    buffer = malloc(sizeof * buffer);
    if (buffer == NULL) {
        fclose(r);
        return FAIL;
    }

    read_input = 0;
    for (i = 0; (read_status = get_signed_int(&tmp, r)) == SUCCESS; i++) {
        tmp_buffer = realloc(buffer, sizeof * buffer * (i + 1));
        if (tmp_buffer == NULL) {
            printf("Error: unable to realloc a block\n");
            fclose(r);
            free(buffer);
            return FAIL;
        }

        buffer = tmp_buffer;
        buffer[i] = tmp;
        read_input = 1;
    }
    fclose(r);

    /* EOF_FAIL is returned on end of line, if no input was read then EOF was sent */
    if (read_status == EOF_FAIL && !read_input) {
        free(buffer);
        return EOF_FAIL;
    } else if (read_status != EOF_FAIL) {
        free(buffer);
        return FAIL;
    }

    arr->arr = buffer;
    arr->length = i;
    return SUCCESS;
}

status get_signed_int(int *ret, FILE *stream)
{
    int sign = 0, seen_sign = 0, seen_digits = 0, c, num, valid = 1, cont = 1;
    sign = seen_sign = seen_digits = num = 0;

    for (; c = getc(stream), cont;) {
        cont = c != EOF && c != '\0' && c != '\n';
        if (!cont) {
            break;
        }

        if (c >= '0' && c <= '9') {
            if (!seen_sign) {
                fprintf(stderr, "Error: you must sign your input with a + or a - before the number\n");
                valid = 0;
                break;
            }

            seen_digits = 1;
            num *= 10;
            num += c - '0';

            if (num > MAX) {
                fprintf(stderr, "Error: your input is larger than the maximum value of %d\n", MAX);
                valid = 0;
                break;
            }
        } else if (c == '+' || c == '-') {
            if (seen_digits && seen_sign) {
                if (c != ungetc(c, stream)) {
                    fprintf(stderr, "Error: ungetc failed\n");
                }
                break;
            } else if (seen_sign) {
                fprintf(stderr, "Error: you cannnot have duplicate signs\n");
                valid = 0;
                break;
            } else if (seen_digits) {
                fprintf(stderr, "Error: you cannot have the sign in the middle of the number\n");
                valid = 0;
                break;
            }

            seen_sign = 1;
            if (c == '-') {
                sign = 1;
            }
        }
    }

    valid &= seen_digits && seen_sign;
    if (valid) {
        if (sign) {
            num *= -1;
        }

        *ret = num;
        return SUCCESS;
    } else if (cont) {
        return FAIL;
    } else {
        return EOF_FAIL;
    }
}
