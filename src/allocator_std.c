#include "allocator_std.h"
#include <stdlib.h>
#include <string.h>

/* Memory allocation function using a function from the standard library*/
static void*
_alloc_std(void *self,
           int bytes)
{
    return malloc(bytes);
}

/* Memory deallocation function using a function from the standard library*/
static void
_dealloc_std(void *self,
             void *addr)
{
    free(addr);
}

/* Memory re-allocation function using a function from the standard library*/
static void*
_realloc_std(void *self,
             void *addr,
             int size)
{
    addr = realloc(addr, size);
    return addr;
}

/* Memory copying function using a function from the standard library*/
static void
_replicate_std(void *self,
               void *dst,
               void *src,
               int size)
{
    memcpy(dst, src, size);
}

/* Allocator initialization function*/
iallocator_t*
allocator_std_new(void *mem)
{
    iallocator_t *allocator_std = (iallocator_t *)mem;
    allocator_std->allocate = _alloc_std;
    allocator_std->deallocate = _dealloc_std;
    allocator_std->reallocate = _realloc_std;
    allocator_std->replicate = _replicate_std;
    return allocator_std;
}
