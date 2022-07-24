#ifndef __INTERFACE_H__
#define __INTERFACE_H__

typedef struct _iallocator_t
{
    void* (*allocate)(void *self, int bytes);
    void (*deallocate)(void *self, void *addr);

    /* pointer to an object that implements this interface */
    void *self;
} iallocator_t;

#endif /* __INTERFACE_H__ */
