#ifndef __TREE_H__
#define __TREE_H__

#include "types.h"
#include "interface.h"

typedef struct _tree_t tree_t;

tree_t *tree_create(int (*compare_fn)(void *, void *), void* (*copy_fn)(void *), void (*dealloc_fn)(void *), iallocator *);
void tree_add_elem(tree_t *t, void *elem);
void tree_print(tree_t *t, void (*print_fn)(void *));
void tree_knot_delete(tree_t *t);
void tree_delete(tree_t *t);

#endif /* __TREE_H__ */
