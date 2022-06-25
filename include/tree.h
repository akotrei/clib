#ifndef __TREE_H__
#define __TREE_H__

#include "types.h"

/** 
 * create new binary tree
 * function return pointer to created binary tree
 * @obj - pointer to a user object that doesn't have a specific data type
 * @compare_fn - pointer to a function that will later compare objects when 
 * added, and will return 1 if @obj1 > @obj2, -1 if @obj1 < @obj2, 0 if @obj1 = @obj2
**/
tree *tree_new(void *obj, int (*compare_fn)(void *obj1, void *obj2));

/**
 * function addition an object to the binary tree
 * @t - pointer to the root of the tree
 * @obj - pointer to a user object which placed in new added node of the tree
**/
void tree_insert(tree *t, void *obj);

/** 
 * print tree function to console
 * @t - pointer to the root of the tree
 * @print_fn - pointer to a function which will be called inside the print 
 * function and what how it will print depends on the implementation of this function
**/
void tree_print(tree *t, void (*print_fn)(void *obj));

#endif // __TREE_H__
