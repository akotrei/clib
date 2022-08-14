#ifndef __INTERFACE_H__
#define __INTERFACE_H__

typedef struct _iallocator_t
{
    void* (*allocate)(void *self, int bytes);
    void (*deallocate)(void *self, void *addr);
    void* (*reallocate)(void *self, void *addr, int size_realloc);
    void (*copy_data)(void *self, void *dest_data, void *src_data, int size_src);

    /* pointer to an object that implements this interface */
    void *self; 
} iallocator_t;

#endif /* __INTERFACE_H__ */
