#include <stdio.h>
#include <stdlib.h>
#include "src/dlist.h"
#include "src/virt_mem.h"
#include "src/utils.h"

int main(void)
{
    virt_mem mem;
    status ret;
    int r;

    ret = init_virt_mem(&mem);
    if (ret == FAIL) {
        fprintf(stderr, "Error: cannot initialise virtual memory\n");
        return 1;
    }

    print_virt_mem(&mem, stdout);

    free_virt_mem(&mem);

    return 0;
}

