#ifndef __TYPES_H__
#define __TYPES_H__

typedef struct container
{
    int alloc_size;             // allocated memory in @elem_size bytes
    int logic_size;             // occupated memory in @elem_size bytes
    int elem_size;              // size of one element in bytes
    void *data;                 // pointer to memory for objects storing
    void (*dealloc_fn)(void *); // pointer to a function that will be invoked while deleting this
                                // container to deallocate objects that were added to this container
                                // if NULL - objects deallocation won't be invoked
    int is_wrapper;             // whether container is a wrapper of some data

} container;

#endif // __TYPES_H__
