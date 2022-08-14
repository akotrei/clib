#include "allocator_std.h"
#include <stdlib.h>
#include <string.h>

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

static void* _realloc_std(void *self, void *addr, int size_realloc)
{	
	addr = realloc(addr, size_realloc);
	return addr;
}

static void _copy_std(void *self, void *dest_data, void *src_data, int size_src)
{
    memcpy(dest_data, src_data, size_src);
}

allocator_std_t *allocator_std_new()
{
    allocator_std_t *allocator_std = (allocator_std_t *)malloc(sizeof(allocator_std_t));
    allocator_std->iallocator.allocate = _alloc_std;
    allocator_std->iallocator.deallocate = _dealloc_std;
    allocator_std->iallocator.reallocate = _realloc_std;
    allocator_std->iallocator.copy_data = _copy_std;
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
