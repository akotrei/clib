#ifndef __INTERFACE_H__
#define __INTERFACE_H__

/* Allocator interface definition*/
typedef struct _iallocator_t
{
    /* Memory allocation function*/
    void* (*allocate)(void *self, int bytes);

    /* Memory deallocation function*/
    void (*deallocate)(void *self, void *addr);

    /* Memory reallocate function*/
    void* (*reallocate)(void *self, void *addr, int size);

    /* Memory copying function*/
    void (*replicate)(void *self, void *dst, void *src, int size);

    /* Pointer to an object that implements this interface */
    void *self;

} iallocator_t;

#endif /* __INTERFACE_H__ */
