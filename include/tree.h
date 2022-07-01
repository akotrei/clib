#ifndef __TREE_H__
#define __TREE_H__

#include "types.h"

typedef struct knot knot;

typedef struct tree tree;

tree *tree_new(int elem_size, void (*dealloc_fn)(void *), int (*compare_fn)(void *, void *));

void tree_add_elem(tree *t, void *elem);

void tree_print(tree *t, void (*print_fn)(void *));

void tree_delete(tree *t);

/*void tree_insert(tree *t, void *obj);*/

/*void tree_print(tree *t, void (*print_fn)(void *obj));*/

/*=========================================================*/
/*typedef struct tree tree;

tree *tree_create(int elem_size, int (*cmp_fn)(void *, void *), void (*dealloc_fn)(void *));
void tree_delete(tree *t);
void tree_add_elem(tree *t, void *elem);
void *tree_find_elem(tree *t, void *elem);
void *tree_delete_elem(tree *t, void *elem);*/

#endif /* __TREE_H__ */
