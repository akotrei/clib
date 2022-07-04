#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "interface.h"

typedef struct allocator_std allocator_std;

allocator_std *allocator_std_new();

void allocator_std_delete(allocator_std *);

iallocator *allocator_std_get_allocator(allocator_std *);

#endif /*__ALLOCATOR_H__*/
