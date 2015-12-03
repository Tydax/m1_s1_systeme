#ifndef __SWAP_H__
#define __SWAP_H__

#include <stdio.h>

extern FILE * swap_file;

char init_swap(const char * path);

char store_to_swap(int vpage, int ppage);

char fetch_from_swap(int vpage, int ppage);

#endif
