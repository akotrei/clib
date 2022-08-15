#ifndef __ARRAY_TYPE_PRIVATE_H__ 
#define __ARRAY_TYPE_PRIVATE_H__ 

/*
 * array type definition
 */
typedef struct _array_t 
{
	
    /* if user passes iallocator equals NULL than array creates internal
     * allocator_std and is owner of this allocator and responsible
     * for deleting this allocator @iallocator_owner equals 1 when
     * array is owner else 0
     */
    int iallocator_owner;	

    /* a variable that is responsible for increasing 
     * the memory for the array when there is not enough of it
     */
    int factor;

    /* the amount of memory to be used by the array
     */
    int alloc_size;

    /* a variable that stores the actual size 
     * of the array that is occupied by user data
     */
    int logic_size;

    /* 
     * array element size
     */
    int elem_size;

    /* 
     * pointer to the beginning of the array data
     */
    void *data;

    /* 
     * pointer to a function to copy an object to an array
     */
    void* (*copy_fn)(void *o);

    /* 
     * pointer to a function to remove an object from an array
     */
    void (*dealloc_fn)(void *o);

    /* 
     * pointer to a function for comparing objects in the array or outside it
     */
    int (*compare_fn)(const void *o1, const void *o2);

    /*
     * pointer to an allocator interface that implements the functions 
     * of allocating, freeing, reallocating, and copying memory
     * note: see file interface.h for details
     */
    iallocator_t *iallocator;

} _array_t;

#endif /* __ARRAY_TYPE_PRIVATE_H__ */
