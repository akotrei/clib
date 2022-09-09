#ifndef __ARRAY_TYPE_PRIVATE_H__
#define __ARRAY_TYPE_PRIVATE_H__

/*
 * Array type definition
 */
typedef struct _array_t
{
    /*
	 * A variable that is responsible for increasing
     * the memory for the array when there is not enough of it
     */
    int factor;

    /* The amount of memory to be used by the array*/
    int alloc_size;

    /*
	 * A variable that stores the actual size
     * of the array that is occupied by user data
     */
    int logic_size;

    /*
     * Array element size
     */
    int elem_size;

    /*
     * Pointer to the beginning of the array data
     */
    void *data;

    /*
     * Pointer to a function to copy an object to an array
     */
    void* (*copy_fn)(void *o);

    /*
     * Pointer to a function to remove an object from an array
     */
    void (*dealloc_fn)(void *o);

    /*
     * Pointer to a function for comparing objects in the array or outside it
     */
    int (*compare_fn)(const void *o1, const void *o2);

    /*
     * Pointer to an allocator interface that implements the functions
     * of allocating, freeing, reallocating, and copying memory
     * note: see file interface.h for details
     */
    iallocator_t *iallocator;

} _array_t;

#endif /* __ARRAY_TYPE_PRIVATE_H__ */
