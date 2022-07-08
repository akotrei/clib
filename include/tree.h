#ifndef __TREE_H__
#define __TREE_H__

#include "types.h"
#include "interface.h"

/*
 * tree type @tree_t declaration
 */
typedef struct _tree_t tree_t;

/*
 * function @tree_create create empty tree and return pointer to created tree
 * @compare_fn    - pointer to function which compare @o1, @o2 and return -1, 
 *                  0 if ">" else return 1, if @compare_fn is equal NULL, then 
 *                  it will not be possible to add knots to the tree 
 * @copy_fn       - pointer to function copy @o transferred this function, if 
 *                  @copy_fn is equal NULL then the tree will store pointers
 *                  to the objects that are added to it, else, objects which 
 *                  will store in the tree will be copies
 * @dealloc_fn    - pointer to function which delete @o transferred this function,
 *                  if @dealloc_fn is equal NULL then will be removed pointers on
 *                  objects in the tree, else, when deleting a tree, the objects
 *                  that are in it will be deleted
 * @il            - pointer to variable(interface of allocator) which has 
 *                  pointers to memory allocation and deallocation functions, if
 *                  @il is equal NULL then allocator create in function 
 *                  @tree_create, else, not create 
*/
tree_t *tree_create(int (*compare_fn)(void *o1, void *o2), void* (*copy_fn)(void *o), void (*dealloc_fn)(void *o), iallocator *il);

/* 
 * function @tree_add_object adds new object in tree creating knot where will store added this object
 * @t - pointer to the tree 
 * @o - pointer to user object which added in tree
*/
void tree_add_object(tree_t *t, void *o);

/*
 * function @tree_delete delete created tree 
 * @t - pointer to the tree 
*/
void tree_delete(tree_t *t);

void *tree_fnd_object(tree_t *t, void *o);
void tree_rmv_object(tree_t *t, void *o);

/* 
 * function @tree_print print in console tree
 * @t - pointer to the tree
 * @print_fn - pointer to print function which user will implement self
 * @o - pointer to object 
*/
void tree_print(tree_t *t, void (*print_fn)(void *o));

#endif /* __TREE_H__ */
