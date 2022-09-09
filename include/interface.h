#ifndef __INTERFACE_H__
#define __INTERFACE_H__

/* allocator interface definition*/
typedef struct _iallocator_t
{
    /* memory allocation function*/
    void* (*allocate)(void *self, int bytes);

    /* memory deallocation function*/
    void (*deallocate)(void *self, void *addr);

    /* memory reallocate function*/
    void* (*reallocate)(void *self, void *addr, int size);

    /* memory copying function*/
    void (*replicate)(void *self, void *dst, void *src, int size);

    /* pointer to an object that implements this interface */
    void *self;

} iallocator_t;

/*
 * Interface of tree iterator type definition
 */
typedef struct _itree_iterator_t
{
    void (*reset)(void *self);
    void* (*get_data)(void *self);
    void* (*curr)(void *self);
    void (*left)(void *self);
    void (*right)(void *self);
    void (*parent)(void *self);
    void *self;
} itree_iterator_t;

#endif /* __INTERFACE_H__ */
