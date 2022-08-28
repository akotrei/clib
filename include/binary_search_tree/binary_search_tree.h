#ifndef __BINARY_SEARCH_TREE_H__
#define __BINARY_SEARCH_TREE_H__

#include "interface.h"
#include "binary_search_tree_type_public.h"

/* DESCRIPTION
 * The basic idea of a binary search tree is that each tree node stores pointers to 
 * its parent and two children (the tree's root node is its own parent). Basic 
 * operations on a tree: creating the tree itself, when it is created, it has no nodes, 
 * that is, it is empty; deleting a tree, from the beginning all nodes in the tree are 
 * deleted, by means of recursion, then the tree itself is deleted, already empty; 
 * adding a node to the tree will be done by comparing the object being added and the 
 * current node, if the object is smaller - it will go to the left subtree, if the 
 * object is larger - the right subtree. With this principle of adding nodes to the tree, 
 * it will look sorted from left to right. The search for a node is performed in the same 
 * way as adding, comparing the current and the node to be found until the required node 
 * is found. Removing a node follows the same principle as adding and searching, but 
 * there are 4 cases of removing a node from the tree:
 * 1) the node being deleted has no sons, that is, the pointers to the nodes 
 * following it are equal to zero;
 * 2) the removed node has a left son, but no right one;
 * 3) the removed node has a right son, but no left one;
 * 4) the removed node has both sons;
 * The first case is solved by simply deleting this node, and the pointers to it 
 * become null;
 * The second case is solved in such a way that the son of the node to be deleted will 
 * be the son of its parent, that is, the son of the node to be deleted will be in 
 * its place;
 * The third case is similar to the third;
 * The fourth case will be solved in such a way that you need to take the right son 
 * of the node to be deleted, and find the leftmost node in the subtree of the right 
 * child of the node to be deleted, and move it to the place of the node to be deleted, 
 * or copy the data in this node to the node to be deleted and delete the leftmost node 
 * of the right subtree of the one being deleted.
 * A binary tree has a balanced property, that is, the difference in the heights of the 
 * subtrees of each vertex of the tree is not less than one, or, the tree has a minimum 
 * height - then it is balanced.
 * A tree, with the help of pointers to the copy and delete functions, has the property 
 * of copying objects to its nodes, or simply storing pointers to these objects. How the 
 * objects will be copied into the tree is decided by the user himself, implementing his 
 * own copy function.
 */

/*
 * The function @tree_create creates an empty tree and returns a pointer 
 * to this created tree
 *
 * @compare_fn    - a pointer to a function that takes two objects to compare 
 *                  and returns 1 if @o1 is less than @o2, -1 if @o1 is 
 *                  greater than @o2, if the objects are equal then 0 is 
 *                  returned. Note: the user must take this into account,
 *                  since the functions of searching, deleting, adding an 
 *                  object will not work correctly
 *
 * @copy_fn       - a pointer to a function that copies the user object to 
 *                  a tree if the copy function is passed when creating 
 *                  a tree, otherwise if NULL is passed, then pointers to 
 *                  user objects will be stored in the knots of the tree, 
 *                  that is, user data will be stored in the form of a tree
 *
 * @dealloc_fn    - a pointer to a function that deletes the user object and 
 *                  the user decides how he will delete this object, if the 
 *                  user does not pass a pointer to his object deletion 
 *                  function to the function of creating a tree and NULL, 
 *                  then the tree will store only pointers to user objects
 *
 * @il            - a pointer to an allocator interface that implements 
 *                  the functions of allocating and freeing memory when 
 *                  creating a tree and its knots. If you pass NULL to this 
 *                  parameter in the tree creation function, then the 
 *                  allocator will be created when the tree is created
*/
tree_t* 
tree_create(int (*compare_fn)(void *o1, void *o2), 
            void* (*copy_fn)(void *o), 
            void (*dealloc_fn)(void *o), 
            iallocator_t *il);

/*
 * the function @tree_delete removes the created tree using recursion
 *
 * @t    - pointer to the tree
*/
void 
tree_delete(tree_t *t);

/* 
 * function @tree_add_object adds a user object to the  tree by creating a new 
 * knot in the tree which will store either a copy of the added object or 
 * a pointer. Adding occurs by comparing the objects that are in the tree with 
 * the one we want to add to it. If the added object is less than the current 
 * one, then it goes to the left subtree of this current knot, otherwise, 
 * to the right subtree
 *
 * @t    - pointer to the tree
 *
 * @o    - pointer to user object which added in the tree
*/
void 
tree_add_object(tree_t *t, 
                void *o);

/*
 * function @tree_fnd_object finds the knot in which the pointer to the user 
 * object is located and returns it, if there is no such object, the NULL is 
 * returned
 *
 * @t    - pointer to the tree
 *
 * @o    - pointer to user object which to be found in the tree
*/
void* 
tree_fnd_object(tree_t *t, 
                void *o);

/*
 * function @tree_rmv_object finds the knot in which the data that the user 
 * wants to delete, the knot is deleted and the function returns a pointer 
 * to the data contained in the deleted knot, otherwise, if there is no knot 
 * to delete, the function returns NULL
 *
 * @t    - pointer to the tree
 *
 * @o    - pointer to user object which to be removed in the tree
*/
void* 
tree_rmv_object(tree_t *t, 
                void *o);

/* 
 * function @tree_print print in console tree using recursion
 *
 * @t           - pointer to the tree
 *
 * @print_fn    - pointer to print function which user will implement self
 *
 * @o           - pointer to object
*/
void 
tree_print(tree_t *t, 
           void (*print_fn)(void *o));

/*
 * @tree_balance_DSW function of tree balance using Day-Stout-Warren algorithm
 *
 * @t    - pointer to the tree
*/
void 
tree_balance_DSW(tree_t *t);

#endif /* __BINARY_SEARCH_TREE_H__ */
