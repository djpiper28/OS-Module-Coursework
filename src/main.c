#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "virt_mem.h"
#include "io_utils.h"

status simulate(virt_mem *mem)
{
    status ret = SUCCESS, remove_ret;
    integer_array arr;
    int i, j, rand_num, val, last_num, left;

    while (ret == SUCCESS) {
        ret = init_integer_array(&arr, stdin);
        if (ret == EOF_FAIL) {
            ret = SUCCESS;
            printf("Info: EOF detected, terminating\n");
            break;
        } else if (ret == FAIL) {
            ret = SUCCESS;
            fprintf(stderr, "Error: invalid input, please try again\n");
        } else {
            for (i = 0; i < arr.length; i++) {
                val = arr.arr[i];
                if (val > 0) {
                    printf("Info: inserting %d nodes\n", val);
                } else if (val == 0) {
                    printf("Info: 0 value was ignored\n");
                } else {
                    printf("Info: removing %d nodes\n", val);
                }

                for (j = 0; j < abs(val) && ret == SUCCESS; j++) {
                    if (val > 0) {
                        rand_num = abs(rand()) % MAX_NODE_INT;
                        rand_num += 1;

                        ret = add_int(mem, rand_num);
                        last_num = rand_num;
                    } else if (val < 0) {
                        left = last_num & 1;

                        remove_ret = remove_int(mem, left);
                        if (remove_ret != SUCCESS) {
                            printf("Info: trying to remove from other side\n");
                            remove_ret = remove_int(mem, !left);
                        }

                        if (remove_ret == FAIL) {
                            printf("Info: cannot remove nodes that do not exist\n");
                            break;
                        }
                    }

                    print_virt_mem(mem, stdout);
                }

                if (ret != SUCCESS) {
                    fprintf(stderr, "Error: cannot perform commands\n");
                }
            }

            print_virt_mem(mem, stdout);

            ret = free_integer_array(&arr);
            if (ret == FAIL) {
                fprintf(stderr, "Error: cannot free integer array\n");
            }
        }
    }

    return ret;
}

int main(int argc, char **argv)
{
    int i, ret_code = 0;
    status ret;
    dlist *current, *last;
    virt_mem virt_mem_list;

    printf("Virtual Memory Simulation, CS2850 Assignment 2\n");
    printf("\tDanny Piper's Submission\n");

    srand(time(NULL));

    ret = init_virt_mem(&virt_mem_list);
    if (ret == FAIL) {
        fprintf(stderr, "Error: cannot initialise virtual memory\n");
        return 1;
    }

    ret = simulate(&virt_mem_list);
    if (ret == FAIL) {
        fprintf(stderr, "Error: an error occurred during simulation of virtual memory\n");
        ret_code = 1;
    }

    ret = free_virt_mem(&virt_mem_list);
    if (ret == FAIL) {
        fprintf(stderr, "Error: cannot free virtual memory\n");
        ret_code = 1;
    }

    return ret_code;
}

