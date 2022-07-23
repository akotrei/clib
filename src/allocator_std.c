#include "allocator_std.h"
#include <stdlib.h>

typedef struct _allocator_std_t
{
    iallocator_t iallocator;
} _allocator_std_t;

static void *_alloc_std(void *self, int bytes)
{
    return malloc(bytes);
}

static void _dealloc_std(void *self, void *addr)
{
    free(addr);
}

allocator_std_t *allocator_std_new()
{
    allocator_std_t *allocator_std = (allocator_std_t *)malloc(sizeof(allocator_std_t));
    allocator_std->iallocator.allocate = _alloc_std;
    allocator_std->iallocator.deallocate = _dealloc_std;
    return allocator_std;
}

void allocator_std_delete(allocator_std_t *allocator_std)
{
    free(allocator_std);
}

iallocator_t *allocator_std_get_allocator(allocator_std_t *allocator_std)
{
    iallocator_t* iallocator = &(allocator_std->iallocator);
    iallocator->self = allocator_std;
    return iallocator;
}
