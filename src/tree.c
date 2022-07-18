#include "tree.h"
#include "allocator_std.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

/*
 * knot type @knot_t declaration
*/
typedef struct _knot_t knot_t;

/*
 * definition @_knot_t type 
*/
typedef struct _knot_t
{
    void*      data;      /*@data      - pointer to the user object that is in the tree knot*/

    knot_t*    left;      /*@left      - pointer to left subtree*/

    knot_t*    right;     /*@right     - pointer to right subtree */

    knot_t*    parent;    /*@parent    - pointer to previous knot*/

} _knot_t;

/*
 * definition @_tree_t type of the tree
*/
typedef struct _tree_t
{
    iallocator*    interface_allocator;                  /*@interface_allocator    - pointer to the allocator interface that is 
                                                                                     responsible for allocating and freeing memory*/

    void*          allocator_standard;                   /*@allocator_standard     - pointer to standard allocator that allocates
                                                                                     and frees memory using @malloc and @free functions*/

    int            (*compare_fn)(void *o1, void *o2);    /*@compare_fn             - pointer to a function that compares two objects 
                                                                                     passed to it and returns the result. For a more 
                                                                                     detailed description of the function, go to the file @tree.h*/

    void*          (*copy_fn)(void *o);                  /*@copy_fn                - pointer to a function that copies the user object to the tree knot*/

    void           (*dealloc_fn)(void *o);               /*@dealloc_fn             - pointer to a function that removes the user object from the tree*/

    knot_t*        knot;                                 /*@knot                   - pointer to the root of the tree*/

} _tree_t;

/*
 * knot creation function @knot_create that returns a pointer to the created knot
 *
 * @data                   - pointer to user object
 *
 * @interface_allocator    - pointer to allocator interface
 *
 * @parent                 - pointer to previous knot
*/
static knot_t* knot_create(void *data, iallocator *interface_allocator, knot_t *parent);

/*
 * function @knot_delete_all_knots to delete all knots in the tree using recursion 
 * 
 * @t             - pointer to the tree
 *
 * @knot          - pointer to pointer to knot in tree
 *
 * @dealloc_fn    - pointer to a user object delete function 
*/
static void knot_delete_all_knots(tree_t *t, knot_t **knot, void (*dealloc_fn)(void *o));

/*
 * a function @copy_fake that returns a pointer to the user object
 *
 * @o    - pointer to user object 
*/
static void* copy_fake(void *o);

/*
 * 
*/
static void knot_print(knot_t *knot, void (*print_fn)(void *o), int level);

tree_t* tree_create(int (*compare_fn)(void *o1, void *o2), void* (*copy_fn)(void *o), void (*dealloc_fn)(void *o), iallocator *interface_allocator)
{
    iallocator *_il;
    allocator_std *al = NULL;
    if(interface_allocator == NULL)
    {
        al = allocator_std_new();
        _il = allocator_std_get_allocator(al);
    }
    else 
    {
        _il = interface_allocator;
    }

    tree_t *t = (tree_t *)_il->allocate(_il, sizeof(tree_t));
    assert(t != NULL && "memory for @tree hasn't been allocated");

    t->interface_allocator = _il;
    t->compare_fn = compare_fn;
    t->dealloc_fn = dealloc_fn;
    t->allocator_standard = (void *)al;
    t->knot = NULL;
    if(copy_fn != NULL)
        t->copy_fn = copy_fn;
    else 
        t->copy_fn = copy_fake;
    return t;
}

void tree_delete(tree_t *t)
{
    knot_delete_all_knots(t, &t->knot, t->dealloc_fn);

    void (*tmp_deallocate)(void *, void *) = t->interface_allocator->deallocate;
    if(t->allocator_standard != NULL)
        allocator_std_delete((allocator_std *)t->allocator_standard);
    tmp_deallocate(NULL, t);
    t = NULL;
}

void tree_add_object(tree_t *t, void *o)
{
    assert(t->compare_fn != NULL && "tree was created with @cpm_fn = NULL");

    knot_t **knot = &t->knot;

    knot_t *tmp_parent = *knot;
    while(*knot != NULL)
    {
        tmp_parent = *knot;
        if (t->compare_fn((*knot)->data, o) <= 0)
            knot = &(*knot)->left;
        else
            knot = &(*knot)->right;
    }

    void *tmp_data = t->copy_fn(o);
    *knot = knot_create(tmp_data, t->interface_allocator, tmp_parent);

    if(tmp_parent == NULL)
        (*knot)->parent = *knot;
}

void* tree_fnd_object(tree_t *t, void *o)
{
    knot_t **knot = &t->knot;
    while(*knot != NULL)
    {
        if(t->compare_fn((*knot)->data, o) < 0) 
            knot = &(*knot)->left;
        if(t->compare_fn((*knot)->data, o) > 0)
            knot = &(*knot)->right;
        if(t->compare_fn((*knot)->data, o) == 0)
            break;
    }

    if(*knot == NULL)
        printf("The knot you were looking for was not found...\n");

    return (void *)(*knot);
}

void* tree_rmv_object(tree_t *t, void *o)
{
    knot_t **knot = &t->knot;
    while(*knot != NULL)
    {
        if(t->compare_fn((*knot)->data, o) < 0) 
            knot = &(*knot)->left;
        if(t->compare_fn((*knot)->data, o) > 0)
            knot = &(*knot)->right;
        if(t->compare_fn((*knot)->data, o) == 0)
        {
            if((*knot)->left == NULL && (*knot)->right == NULL)
            {
                void *data_of_deleting_knot = (*knot)->data;
                t->interface_allocator->deallocate(NULL, *knot);
                *knot = NULL;
                return data_of_deleting_knot;
            }
            
            if((*knot)->left != NULL && (*knot)->right == NULL)
            {
                void *data_of_deleting_knot = (*knot)->data;
                knot_t* tmp = *knot;
                *knot = (*knot)->left;
                (*knot)->parent = tmp->parent;
                t->interface_allocator->deallocate(NULL, tmp);
                tmp = NULL;
                return data_of_deleting_knot;
            }

            if((*knot)->right != NULL && (*knot)->left == NULL)
            {
                void *data_of_deleting_knot = (*knot)->data;
                knot_t* tmp = *knot;
                *knot = (*knot)->right;
                (*knot)->parent = tmp->parent;
                t->interface_allocator->deallocate(NULL, tmp);
                tmp = NULL;
                return data_of_deleting_knot;
            }

            if((*knot)->left != NULL && (*knot)->right != NULL)
            {
                knot_t **tmp = &(*knot)->right; 

                while((*tmp)->left != NULL)
                    tmp = &(*tmp)->left;
                
                void *data_of_deleting_knot = (*knot)->data;
                (*knot)->data = (*tmp)->data;
                (*tmp)->data = data_of_deleting_knot;
                knot_t *deleting_knot = *tmp;

                if((*tmp)->right != NULL)
                {
                    *tmp = (*tmp)->right;
                    (*tmp)->parent = deleting_knot->parent;
                    t->interface_allocator->deallocate(NULL, deleting_knot);
                    deleting_knot = NULL;
                    return data_of_deleting_knot;
                }
                if((*tmp)->right == NULL) 
                {
                    t->interface_allocator->deallocate(NULL, *tmp);
                    *tmp = NULL;
                    return data_of_deleting_knot;
                }
            }
        }
    }
    return NULL;
}

void tree_print(tree_t *t, void (*print_fn)(void *o))
{
    knot_print(t->knot, print_fn, 0);
}

static knot_t *knot_create(void *data, iallocator *interface_allocator, knot_t *parent)
{
    knot_t *knot;
    knot = (knot_t *)interface_allocator->allocate(interface_allocator, sizeof(knot_t));
    assert(knot != NULL && "memory for @knot hasn't been allocated");

    knot->data = data;
    knot->left = knot->right = NULL;
    knot->parent = parent;
    return knot;
}

static void knot_print(knot_t *knot, void (*print_fn)(void *o), int level)
{
    if(knot != NULL)
    {
        knot_print(knot->right, print_fn, level + 1);
        if(level != 0)
        {
            for(int i = 0; i < level - 1; i++)
                printf("|\t");
            printf("|------"); print_fn(knot->data); printf("\n");
        }
        else 
        {
            print_fn(knot->data); printf("\n");
        }
        knot_print(knot->left, print_fn, level + 1);
    }
}

static void knot_delete_all_knots(tree_t *t, knot_t **knot, void (*dealloc_fn)(void *o))
{
    if(*knot != NULL)
    {
        knot_delete_all_knots(t, &(*knot)->left, dealloc_fn);
        knot_delete_all_knots(t, &(*knot)->right, dealloc_fn);
        if(dealloc_fn != NULL)
            dealloc_fn((*knot)->data);
        t->interface_allocator->deallocate(NULL, *knot);
    }
}

static void *copy_fake(void *o) { return o; }

static int bst_to_vine(knot_t *grand)
{
    int count = 0;
    knot_t *tmp = grand->right;
    while(tmp != NULL)
    {
        if(tmp->left != NULL)
        {
            knot_t *oldtmp = tmp;
            tmp = tmp->left;
            oldtmp->left = tmp->right;
            tmp->right = oldtmp;
            grand->right = tmp;
        }
        else 
        {
            count++;
            grand = tmp;
            tmp = tmp->right;
        }
    }
    return count;
}

static void compress(knot_t *grand, int m)
{
    knot_t *tmp = grand->right;
    for(int i = 0; i < m; i++)
    {
        knot_t *oldtmp = tmp;
        tmp = tmp->right;
        grand->right = tmp;
        oldtmp->right = tmp->left;
        tmp->left = oldtmp;
        grand = tmp;
        tmp = tmp->right;
    }
}

static knot_t* tree_balance(tree_t *t)
{
    knot_t *grand = knot_create(NULL, t->interface_allocator, NULL); 
    grand->right = t->knot;
    int count = bst_to_vine(grand);
    int h = log2((double)(count + 1));
    int m = pow(2, h) - 1;
    compress(grand, count - m);
    for(m = m / 2; m > 0; m /= 2)
        compress(grand, m);
    knot_t *result = grand->right;
    t->interface_allocator->deallocate(NULL, grand);
    return result;
}

void tree_balance_DSW(tree_t *t) { t->knot = (knot_t *)tree_balance(t); }
