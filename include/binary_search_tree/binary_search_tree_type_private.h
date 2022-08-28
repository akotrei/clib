#ifndef __BINARY_SEARCH_TREE_TYPE_PRIVATE_H__
#define __BINARY_SEARCH_TREE_TYPE_PRIVATE_H__

/* DESCRIPTION
 * This file defines types @tree_t, @knot_t for working with a tree and its nodes.
 * The tree uses the allocator interface, which is responsible for allocating, freeing, 
 * re-allocating, copying the memory that is needed to work with the tree in order 
 * to create it, copy user data into it, delete data from it. The tree contains 
 * a variable responsible for the fact that if the allocator was created in the tree, 
 * it will be deleted when the tree is deleted, otherwise the tree will simply be 
 * deleted. The tree has pointers to functions that are responsible for copying and 
 * deleting the object that the user will add to the tree. The main function that 
 * will be used in the tree is the object comparison function, since the idea of the 
 * main idea of the tree is to add objects to nodes by comparing them, and in order 
 * to abstract from all data types, there is a pointer to the object comparison function 
 * in the tree, which is implemented by the user himself, and he will decide by what 
 * criteria his objects will be compared for their further interaction with the tree.
 * The tree also has a pointer to its root node, from which interaction with all 
 * nodes of the tree begins. 
 * In knot @knot_t type the node type declares pointers to data, parent node, right 
 * and left child of the node. The data is declared as an independent void type to 
 * keep the abstraction from any data type.
 */

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
