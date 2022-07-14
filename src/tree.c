#include "tree.h"
#include "allocator_std.h"
#include "itree.h"

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
    void*      data;      /*@data   - pointer to object which will store in this struct*/
    knot_t*    left;      /*@left   - pointer to left subtree*/
    knot_t*    right;     /*@right  - pointer to right subtree*/
    knot_t*    parent;    /*@parent - pointer to previous knot*/
}_knot_t;

/*
 * definition @_tree_t type of the tree
*/
typedef struct _tree_t
{
    iallocator*    il;                              /*@il         - pointer to interface of allocator which can allocate and deallocate memory use functions*/
    void*          allocator;                       /*@allocator  - pointer to standard allocator which have functions @malloc and @free 
                                                                    for allocate and deallocate memory*/
    int            (*compare_fn)(void *, void *);   /*@compare_fn - pointer to function that can compare transferred her objects. 
                                                                    This function should implement user, because he decided how objects should be compared. 
                                                                    You can find out what result the function returns by looking at the comments in the file @tree.h*/
    void*          (*copy_fn)(void *);              /*@copy_fn    - pointer to function that return pointer to copy of object which was transferred of user and
                                                                    thereafter knot store copy of user object. More detailed description you can find in the file @tree.h*/
    void           (*dealloc_fn)(void *);           /*@dealloc_fn - pointer to function that implement user for delete object*/
    knot_t*        knot;                            /*@knot       - pointer to knot of the tree*/
    itree          it;                              /*@it         - variable of interface of tree, with which possible use balance for the tree.
                                                                    Details about the interface can be found in the file @itree.h*/
}_tree_t;

/* 
 * function @knot_create for create knot in the tree and return pointer to created knot 
 * @data      - pointer to data which store knot in the tree
 * @il        - pointer to interface of allocator that will allocate memory for knot 
 * @parent    - pointer to previous knot
*/
static knot_t *knot_create(void *data, iallocator *il, knot_t *parent);

/*
 * function @knot_delete_all_knots for deleting all knots in the tree 
 * @t             - pointer to the tree 
 * @knot          - pointer to the knot 
 * @dealloc_fn    - pointer to function that delete user object
 * @o             - pointer to deleting user object
*/
static void knot_delete_all_knots(tree_t *t, knot_t **knot, void (*dealloc_fn)(void *o));

/*
 * function @copy_fake return pointer to user object 
 * @o    - pointer to user object
*/
static void* copy_fake(void *o);

/*
 * function for printing tree in console 
 * @knot        - pointer to the knot
 * @print_fn    - pointer to function implemented by the user that print @o object in a console
 * @level       - a variable that stores tha height of the tree
*/
static void knot_print(knot_t *knot, void (*print_fn)(void *o), int level);

tree_t *tree_create(int (*compare_fn)(void *o1, void *o2), void* (*copy_fn)(void *o), void (*dealloc_fn)(void *o), iallocator *il)
{
    // pointer to interface of allocator 
    iallocator *_il;

    // pointer to standard allocator 
    allocator_std *al = NULL;

    // if user passed to function @il is equal NULL, then create standard(malloc and free) allocator for allocate of memory
    if(il == NULL)
    {
        // create standard allocator 
        al = allocator_std_new();

        // cast standard allocator to type interface to use functions of allocate and freeing memory
        _il = allocator_std_get_allocator(al);
    }
    else 
    {
        // if user passed to function @il is not equal NULL
        _il = il;
    }

    // create the tree with help interface of allocator 
    tree_t *t = (tree_t *)_il->allocate(_il, sizeof(tree_t));
    assert(t != NULL && "memory for @tree hasn't been allocated");

    // tree structure field initialization 
    t->il = _il;
    t->compare_fn = compare_fn;
    t->dealloc_fn = dealloc_fn;
    t->allocator = (void *)al;
    t->it.self = t;
    t->it.get_data = tree_get_data;
    t->it.get_left = tree_get_left_subtree;
    t->it.get_right = tree_get_right_subtree;
    t->it.get_parent = tree_get_parent_knot;
    t->it.add_elem = tree_add_object;
    t->it.del_elem = tree_rmv_object;
    t->it.fnd_elem = tree_fnd_object;
    t->knot = NULL;

    // function of copy object, if @copy_fn is equal NULL then user objects will be copied in this tree, else, the tree will be store pointers on this objects
    if(copy_fn != NULL)
        t->copy_fn = copy_fn;
    else 
        t->copy_fn = copy_fake;

    // return created the tree 
    return t;
}

void tree_delete(tree_t *t)
{
    knot_delete_all_knots(t, &t->knot, t->dealloc_fn);
    void (*ptf)(void *, void *) = t->il->deallocate;
    if(t->allocator != NULL)
        allocator_std_delete((allocator_std *)t->allocator);
    ptf(NULL, t);
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
    *knot = knot_create(tmp_data, t->il, tmp_parent);

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

void tree_rmv_object(tree_t *t, void *o)
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
                if(t->dealloc_fn != NULL)
                    t->dealloc_fn((*knot)->data);
                t->il->deallocate(NULL, *knot);
                *knot = NULL;
                break;
            }

            if((*knot)->left != NULL && (*knot)->right == NULL)
            {
                *knot = (*knot)->left;
                if(t->dealloc_fn != NULL)
                    t->dealloc_fn((*knot)->parent->data);
                t->il->deallocate(NULL, (*knot)->parent);
                (*knot)->parent = NULL;
                break;
            }

            if((*knot)->right != NULL && (*knot)->left == NULL)
            {
                *knot = (*knot)->right;
                if(t->dealloc_fn != NULL)
                    t->dealloc_fn((*knot)->parent->data);
                t->il->deallocate(NULL, (*knot)->parent);
                (*knot)->parent = NULL;
                break;
            }

            if((*knot)->left != NULL && (*knot)->right != NULL)
            {
                knot_t **tmp = &(*knot)->right; 

                while((*tmp)->left != NULL)
                    tmp = &(*tmp)->left;
                
                int so = sizeof((*tmp)->data);
                void *tmp_data = (void *)malloc(so);
                memcpy(tmp_data, (*tmp)->data, so);
                (*knot)->data = tmp_data;

                if((*tmp)->right != NULL)
                {
                    *tmp = (*tmp)->right;
                    (*tmp)->parent = NULL;
                }

                if(t->dealloc_fn != NULL)
                    t->dealloc_fn((*tmp)->data);
                t->il->deallocate(NULL, *tmp);
                *tmp = NULL;
                break;
            }
            break;
        }
    }
}

void* tree_get_data(void *knot)
{
    knot_t *tmp_knot = (knot_t *)knot;
    return tmp_knot->data;
}

void* tree_get_left_subtree(void *knot)
{
    knot_t *tmp_knot = (knot_t *)knot;
    return tmp_knot->left;
}

void* tree_get_right_subtree(void *knot)
{
    knot_t *tmp_knot = (knot_t *)knot;
    return tmp_knot->right;
}

void* tree_get_parent_knot(void *knot)
{
    knot_t *tmp_knot = (knot_t *)knot;
    return tmp_knot->parent;
}

void tree_print(tree_t *t, void (*print_fn)(void *o))
{
    knot_print(t->knot, print_fn, 0);
}

static knot_t *knot_create(void *data, iallocator *il, knot_t *parent)
{
    knot_t *knot;
    knot = (knot_t *)il->allocate(il, sizeof(knot_t));
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
        t->il->deallocate(NULL, *knot);
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
    knot_t *grand = knot_create(NULL, t->il, NULL); 
    grand->right = t->knot;
    int count = bst_to_vine(grand);
    int h = log2((double)(count + 1));
    int m = pow(2, h) - 1;
    compress(grand, count - m);
    for(m = m / 2; m > 0; m /= 2)
        compress(grand, m);
    knot_t *result = grand->right;
    t->il->deallocate(NULL, grand);
    return result;
}

void tree_balance_algorithm_DSW(tree_t *t) { t->knot = (knot_t *)tree_balance(t); }
