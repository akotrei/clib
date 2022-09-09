#ifndef __ALLOCATOR_STD_H__
#define __ALLOCATOR_STD_H__

#include "interface.h"

/*
 * Function of creating a standard(which using functions from stdlib.h for
 * working with memory) allocator that will be responsible for working with
 * memory(allocate, deallocate, reallocate, copying data).
 * Parameter @mem is a pointer to per block of memory that will be used to
 * initialize the allocator.
 */
iallocator_t*
allocator_std_new(void *mem);

#endif /* __ALLOCATOR_STD_H__ */
