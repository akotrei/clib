#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "interface.h"

typedef struct _allocator_std_t allocator_std_t;

allocator_std_t *allocator_std_new();

void allocator_std_delete(allocator_std_t *);

iallocator_t *allocator_std_get_allocator(allocator_std_t *);

#endif /*__ALLOCATOR_H__*/
