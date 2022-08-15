#ifndef __BINARY_SEARCH_TREE_TYPE_PRIVATE_H__
#define __BINARY_SEARCH_TREE_TYPE_PRIVATE_H__

/*
 * definition @_tree_t type of the tree
 */
typedef struct _tree_t
{
    /* 
     * pointer to iallocator interface that is responsible
     * for allocating and deallocating internal data in tree when user modifies
     * a tree
     */ 
    iallocator_t *iallocator;

    /* if user passes iallocator equals NULL than tree creates interbal allocator_std
     * and is owner of this allocator and responsible for deleting this allocator
     * @iallocator_owner equals 1 when tree is owner else 0
     */
    int iallocator_owner; 

    /* pointer to a function that compares two objects are
     * passed to it and returns the result. For a more
     * detailed description of the function, go to the file tree.h
     */
    int (*compare_fn)(void *o1, void *o2);

    /* pointer to a function that copies the user object to the tree knot*/
    void *(*copy_fn)(void *o); 

    /* pointer to a function that removes the user object from the tree*/
    void (*dealloc_fn)(void *o); 
    
    /* pointer to the root of the tree*/
    knot_t *knot; 

} _tree_t;

/*
 * definition @_knot_t type
 */
typedef struct _knot_t
{
    /* pointer to the user object */
    void *data;

    /* pointer to left knot*/
    knot_t *left;

    /* pointer to right knot*/
    knot_t *right;

    /* pointer to previous knot*/
    knot_t *parent;

} _knot_t;

#endif /* __BINARY_SEARCH_TREE_TYPE_PRIVATE_H__ */
