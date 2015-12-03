#include <swap.h>

#include <stdio.h>
#include <kernel.h>

swap_file = NULL;

char init_swap(const char * path) {
    char res;
    res = -1;

    if (!swap_file) {
        swap_file = fopen(path, "w+");

        if (swap_file) {
            res = 0;
        }
    }

    return res;
}

char store_to_swap(int vpage, int ppage) {
    char res;
    res = -1;

    if (!fseek(swap_file, vpage * PAGE_SIZE, SEEK_SET)) {
        if (fwrite(physical_memory + ppage * PAGE_SIZE, PAGE_SIZE, 1)) {
            res = 0;
        }
    }

    return res;
}

char fetch_from_swap(int vpage, int ppage) {
    char res;
    res = -1;

    if (!fseek(swap_file, vpage * PAGE_SIZE, SEEK_SET)) {
        res = 0;
        if (!fread(physical_memory + ppage * PAGE_SIZE, PAGE_SIZE, 1)) {
            memset(physical_memory + ppage * PAGE_SIZE, 1, PAGE_SIZE);
        }
    }

    return res;
}
