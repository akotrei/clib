#ifndef __TREE_H__
#define __TREE_H__

#include "types.h"
#include "interface.h"

typedef struct _tree_t tree_t;

tree_t *tree_create(int (*compare_fn)(void *o1, void *o2), void* (*copy_fn)(void *o), void (*dealloc_fn)(void *o), iallocator *il);
void tree_add_object(tree_t *t, void *o);
void tree_print(tree_t *t, void (*print_fn)(void *o));
void tree_delete(tree_t *t);

#endif /* __TREE_H__ */
