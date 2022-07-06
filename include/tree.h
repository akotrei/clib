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
 * @compare_fn    - pointer to function which compare @o1, @o2 and 
 *                  return -1, 0 if ">" else return 1
 * @copy_fn       - pointer to function copy @o transferred this function
 * @dealloc_fn    - pointer to function which delete @o transferred this function
 * @il            - pointer to variable(interface of allocator) having functions 
 *                  allocate, deallocate which SELECTION and FREE memory 
 *                  respectively
*/
tree_t *tree_create(int (*compare_fn)(void *o1, void *o2), void* (*copy_fn)(void *o), void (*dealloc_fn)(void *o), iallocator *il);

/* 
 * function @tree_add_object adds new object in tree creating knot where will store added this object
 * @t - pointer to the tree 
 * @o - pointer to user object which added in tree
*/
void tree_add_object(tree_t *t, void *o);

/* 
 * function @tree_print print in console tree
 * @t - pointer to the tree
 * @print_fn - pointer to print function which user will implement self
 * @o - pointer to object 
*/
void tree_print(tree_t *t, void (*print_fn)(void *o));

/*
 * function @tree_delete delete created tree 
 * @t - pointer to the tree 
*/
void tree_delete(tree_t *t);

#endif /* __TREE_H__ */
