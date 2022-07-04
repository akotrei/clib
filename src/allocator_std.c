#include "allocator_std.h"
#include <stdlib.h> 

typedef struct allocator_std 
{
    iallocator il;
};

static void *_alloc_std(void *self, int bytes)
{
    return malloc(bytes);
}

static void _dealloc_std(void *self, void *addr)
{
    free(addr);
}

allocator_std *allocator_std_new()
{
    allocator_std *al = (allocator_std *)malloc(sizeof(allocator_std));
    al->il.allocate = _alloc_std;
    al->il.deallocate = _dealloc_std;
    return al;
}

void allocator_std_delete(allocator_std *allocator)
{
    free(allocator);
}

iallocator *allocator_std_get_allocator(allocator_std *allocator)
{
    return &allocator->il;
}
