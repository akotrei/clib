#ifndef __TREE_H__
#define __TREE_H__

#include "types.h"
#include "interface.h"

/*
 *  tree type @tree_t declaration
 */
typedef struct _tree_t tree_t;

/*
 * The function @tree_create creates an empty tree and returns a pointer to this created tree
 *
 * @compare_fn    - a pointer to a function that takes two objects to compare and returns 1
 *                  if @o1 is less than @o2, -1 if @o1 is greater than @o2, if the objects
 *                  are equal then 0 is returned. Note: the user must take this into account,
 *                  since the functions of searching, deleting, adding an object will not work correctly
 *
 * @copy_fn       - a pointer to a function that copies the user object to a tree if the copy
 *                  function is passed when creating a tree, otherwise if NULL is passed, then
 *                  pointers to user objects will be stored in the knots of the tree, that is,
 *                  user data will be stored in the form of a tree
 *
 * @dealloc_fn    - a pointer to a function that deletes the user object and the user decides how he will
 *                  delete this object, if the user does not pass a pointer to his object deletion function
 *                  to the function of creating a tree and NULL, then the tree will store
 *                  only pointers to user objects
 *
 * @il            - a pointer to an allocator interface that implements the functions of allocating and
 *                  freeing memory when creating a tree and its knots. If you pass NULL to this
 *                  parameter in the tree creation function, then the allocator will be created
 *                  when the tree is created
 */
tree_t*
tree_create(int (*compare_fn)(void *o1, void *o2),
            void* (*copy_fn)(void *o),
            void (*dealloc_fn)(void *o),
            iallocator_t *iallocator);

/*
 * the function @tree_delete remove tree @t
 *
 * @t    - pointer to the tree
 */
void tree_delete(tree_t *t);

/*
 * function @tree_add_object adds a user object to the  tree by creating a new knot
 * in the tree which will store either a copy of the added object or a pointer. Adding
 * occurs by comparing the objects that are in the tree with the one we want to add
 * to it. If the added object is less than the current one, then it goes to the left subtree
 * of this current knot, otherwise, to the right subtree
 *
 * @t    - pointer to the tree
 *
 * @o    - pointer to user object which added in the tree
 */
void tree_add_object(tree_t *t, void *o);

/*
 * function @tree_fnd_object finds the knot in which the pointer to the user object is
 * located and returns it, if there is no such object, the NULL is returned
 *
 * @t    - pointer to the tree
 *
 * @o    - pointer to user object which to be found in the tree
 */
void* tree_fnd_object(tree_t *t, void *o);

/*
 * function @tree_rmv_object finds the knot in which the data that the user wants to
 * delete, the knot is deleted and the function returns a pointer to the data contained
 * in the deleted knot, otherwise, if there is no knot to delete, the function returns NULL
 *
 * @t    - pointer to the tree
 *
 * @o    - pointer to user object which to be removed in the tree
 */
void* tree_rmv_object(tree_t *t, void *o);

/*
 * function @tree_print print in console tree using recursion
 *
 * @t           - pointer to the tree
 *
 * @print_fn    - pointer to print function which user will implement self
 *
 * @o           - pointer to object
 */
void tree_print(tree_t *t, void (*print_fn)(void *o));

/*
 * function of tree balance using Day-Stout-Warren algorithm
 *
 * @t    - pointer to the tree
 */
void tree_balance_DSW(tree_t *t);

knot_t* tree_get_root(tree_t* t);

knot_t* knot_get_left(knot_t* knot);
knot_t* knot_get_right(knot_t* knot);
knot_t* knot_get_parent(knot_t* knot);

void f(knot_t* t, array_t* a)
{
        if (t==NULL)
        {
                return;
        }
        array_push_back(a, t->data);
        f(knot_get_left(t));
        f(knot_get_right(t));

}
void* knot_get_data(knot_t* knot);

#endif /* __TREE_H__ */
