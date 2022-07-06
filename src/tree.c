#include "tree.h"
#include "allocator_std.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
 * interface @itree declaration
*/
typedef struct itree itree;

/*
 * interface @itree definition
*/
typedef struct itree 
{
    void*  (*get_data)(void *self);
    itree* (*get_left)(void *self);
    itree* (*get_right)(void *self);
    void   (*add_object)(void *self, void *o);
    void   (*del_object)(void *self, void *o);
    void   (*fnd_object)(void *self, void *o);
};

/*
 * knot type @knot_t declaration
*/
typedef struct _knot_t knot_t;

/*
 * definition @_knot_t type 
*/
typedef struct _knot_t
{
    void *data;       /*@data  - pointer to object which will store in this struct*/
    knot_t *left;     /*@left  - pointer to left subtree*/
    knot_t *right;    /*@right - pointer to right subtree*/
};

/*
 * definition @_tree_t type of the tree
*/
typedef struct _tree_t
{
    iallocator *il;                       /*@il         - */
    void *allocator;                      /*@allocator  - */
    int (*compare_fn)(void *, void *);    /*@compare_fn - */
    void* (*copy_fn)(void *);             /*@copy_fn    - */
    void (*dealloc_fn)(void *);           /*@dealloc_fn - */
    knot_t *knot;                         /*@knot       - */
};

static knot_t *knot_create(void *data, iallocator *il);
static void knot_print(knot_t *knot, void (*print_fn)(void *));
static void knot_delete(knot_t *knot, void (*dealloc_fn)(void *data));
static void knot_delete_all(tree_t *t, knot_t **knot, void (*dealloc_fn)(void *o));
static void *pointer_to_object(void *o);

tree_t *tree_create(int (*compare_fn)(void *o1, void *o2), void* (*copy_fn)(void *o), void (*dealloc_fn)(void *o), iallocator *il)
{
    iallocator *_il;
    allocator_std *al = NULL;

    if(!il)
    {
        al = allocator_std_new();
        _il = allocator_std_get_allocator(al);
    }
    else 
    {
        _il = il;
    }

    tree_t *t = (tree_t *)_il->allocate(_il, sizeof(tree_t));
    assert(t != NULL && "memory for @tree hasn't been allocated");

    t->il = _il;
    t->compare_fn = compare_fn;
    t->dealloc_fn = dealloc_fn;
    t->allocator = (void *)al;

    if(copy_fn != NULL)
        t->copy_fn = copy_fn;
    else 
        t->copy_fn = pointer_to_object;

    t->knot = NULL;
    return t;
}

void tree_delete(tree_t *t)
{
    knot_delete_all(t, &t->knot, t->dealloc_fn);
    if(t->allocator != NULL)
       allocator_std_delete((allocator_std *)t->allocator);
    t->il->deallocate(NULL, t);
    
}

void tree_add_object(tree_t *t, void *o)
{
    assert(t->compare_fn != NULL && "tree was created with @cpm_fn = NULL");
    knot_t **knot = &t->knot;
    while(*knot != NULL)
    {
        if (t->compare_fn((*knot)->data, o) <= 0)
            knot = &(*knot)->left;
        else
            knot = &(*knot)->right;
    }
    void *tmp_data = t->copy_fn(o);
    *knot = knot_create(tmp_data, t->il);
}

void tree_print(tree_t *t, void (*print_fn)(void *o))
{
    knot_print(t->knot, print_fn);
}

static knot_t *knot_create(void *data, iallocator *il)
{
    knot_t *knot;
    knot = (knot_t *)il->allocate(il, sizeof(knot_t));
    assert(knot != NULL && "memory for @knot hasn't been allocated");

    knot->data = data;
    knot->left = knot->right = NULL;
    return knot;
}

static void knot_print(knot_t *knot, void (*print_fn)(void *o))
{
    if(knot == NULL)
        return;

    knot_print(knot->left, print_fn);
    print_fn(knot->data);
    knot_print(knot->right, print_fn);
}

static void knot_delete_all(tree_t *t, knot_t **knot, void (*dealloc_fn)(void *o))
{
    if(*knot == NULL)
        return; 

    knot_delete_all(t, &(*knot)->left, dealloc_fn);
    knot_delete_all(t, &(*knot)->right, dealloc_fn);
    dealloc_fn((*knot)->data);
    t->il->deallocate(NULL, *knot);
}

static void knot_delete(knot_t *knot, void (*dealloc_fn)(void *data))
{
    dealloc_fn(knot->data);
    free(knot);
}

static void *pointer_to_object(void *o)
{
    return o;
}
