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

/*
 * Interface of list iterator type definition
 */
typedef struct _ilist_iterator_t
{
    /* Sets a pointer to the head of the list*/
    void (*begin)(void *self);

    /* Sets a pointer to the tail of the list*/
    void (*end)(void *self);

    /* Get the date from the list node*/
    void* (*get_data)(void *self);

    /* Returns the current node*/
    void* (*curr)(void *self);

    /* moving to the next node*/
    void (*next)(void *self);

    /* moving to the previous node*/
    void (*prev)(void *self);

    /* Pointer to an object that implements this interface */
    void *self;

} ilist_iterator_t;

#endif /* __INTERFACE_H__ */
