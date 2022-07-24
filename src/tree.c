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
    /* pointer to the user object that is in the tree knot */
    void* data;   

    /* pointer to left subtree */
    knot_t* left;   

    /* pointer to right subtree */
    knot_t* right;

    /* pointer to previous knot */
    knot_t* parent;

} _knot_t;

/*
 * definition @_tree_t type of the tree
 */
typedef struct _tree_t
{  
    /*
     * pointer to iallocator interface that is responsible
     * for allocating and deallocating internal data in tree when user modifies
     * a tree
     */ 
    iallocator_t* iallocator;        
    
    /* if user passes iallocator equals NULL than tree creates internal 
     * allocator_std and is owner of this allocator and responsible 
     * for deleting this allocator @iallocator_owner equals 1 when 
     * tree is owner else 0
     */
    int iallocator_owner;
    
    /* pointer to a function that compares two objects are
     * passed to it and returns the result. For a more
     * detailed description of the function, go to the file tree.h
     */
    int (*compare_fn)(void *o1, void *o2);

    /* pointer to a function that copies the user object to the tree knot */
    void* (*copy_fn)(void *o);   

    /* pointer to a function that removes the user object from the tree */
    void (*dealloc_fn)(void *o);

    /* pointer to the root of the tree */
    knot_t* knot;        
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
static knot_t* knot_create(void *data, iallocator_t *interface_allocator, knot_t *parent);

/*
 * a function @tree_delete_all_knots to delete all knots 
 * in the tree using recursion 
 * 
 * @t             - pointer to the tree
 *
 * @knot          - pointer to pointer to knot in tree
 *
 * @dealloc_fn    - pointer to a user object delete function 
 */
static void tree_delete_all_knots(knot_t **knot, void (*dealloc_fn)(void *o), void (*deallocate)(void *self, void *address));

/*
 * a function @copy_fake that returns a pointer to the user object
 *
 * @o    - pointer to user object 
 */
static void* copy_fake(void *o);

static void dealloc_fake(void *o) {};

/*
 * 
 */
static void knot_print(knot_t *knot, void (*print_fn)(void *o), int level);

/*
 * function @tree_create creates a tree and returns a pointer to it
 */
tree_t* tree_create(int (*compare_fn)(void *o1, void *o2), void* (*copy_fn)(void *o), void (*dealloc_fn)(void *o), iallocator_t *interface_allocator)
{
    /*pointer to allocator interface*/
    iallocator_t *_iallocator;

    /*pointer to standard allocator*/
    allocator_std_t *iallocator_std = NULL;

    int _iallocator_owner;
    
    /* if the pointer to the allocator interface passed to 
     * the function creating a tree is NULL then a standard allocator is created
     */
    if(interface_allocator == NULL)
    {
        /*creating a standard allocator*/
        iallocator_std = allocator_std_new();

        /*getting interface allocator from standard allocator*/
        _iallocator = allocator_std_get_allocator(iallocator_std);
        _iallocator_owner = 1;
    }
    /*if the pointer to the allocator interface passed 
     * to the function creating a tree is not NULL
     */
    else 
    {
        /*allocator interface assignment*/
        _iallocator = interface_allocator;
        _iallocator_owner = 0;
    }

    /*creating a tree using the allocator interface*/
    tree_t *t = (tree_t *)_iallocator->allocate(_iallocator->self, 
                                                sizeof(tree_t));

    /*tree structure field assignment*/
    t->iallocator = _iallocator;
    t->iallocator_owner = _iallocator_owner;

    assert(compare_fn != NULL && "@compare_fn mustn't be NULL");
    t->compare_fn = compare_fn;

    t->knot = NULL;

    if(dealloc_fn != NULL)
        t->dealloc_fn = dealloc_fn;
    else 
        t->dealloc_fn = dealloc_fake;

    if(copy_fn != NULL)
        t->copy_fn = copy_fn;
    else 
        t->copy_fn = copy_fake;

    /*return a pointer to the created tree*/
    return t;
}

/*
 * function @tree_delete removes the tree
 */
void tree_delete(tree_t *t)
{
    /*pointer to a function that removes the tree itself*/
    void (*deallocate)(void *self, 
                       void *address) = t->iallocator->deallocate;

    /*a function that recursively removes all knots of a tree*/
    tree_delete_all_knots(&t->knot, t->dealloc_fn, deallocate);

    /*if the standard allocator has not been removed up to this moment*/
    if(t->iallocator_owner == 1)
    {
        /*deleting the standard allocator*/
        allocator_std_delete(t->iallocator->self);
    }

    /*tree removal*/
    deallocate(NULL, t);
}

/*
 * function @tree_add_object to add an object to the tree
 */
void tree_add_object(tree_t *t, void *o)
{
    /*pointer to pointer to tree root*/
    knot_t **knot = &t->knot;

    /*pointer to previous knot*/
    knot_t *tmp_parent = *knot;

    /*traversing a tree with a pointer to pointer in a loop */
    while(*knot != NULL)
    {
        /*pointer to the previous knot in the tree, 
         * this is necessary in order to save this pointer in the created knot*/
        tmp_parent = *knot;

        /*if the objects are equal then exit the function*/
        if (t->compare_fn((*knot)->data, o) == 0)
        {
            /*function exit*/
            return;
        }

        /*if the object being added is less than the current one, 
         * go to the left subtree*/
        if (t->compare_fn((*knot)->data, o) < 0)
        {
            /*move to left subtree*/
            knot = &(*knot)->left;
        }
        /*if the object being added is greater than the current one, 
         * go to the right subtree*/
        else
        {
            /*move to right subtree*/
            knot = &(*knot)->right;
        }
    }

    /*created knot*/
    *knot = knot_create(t->copy_fn(o), t->iallocator, tmp_parent);

    /*when adding the first knot(root knot) to the tree, 
     * its pointer to the previous pointer points to the root of the tree itself*/
    if(tmp_parent == NULL)
    {
        /*assigning the root knot to the parent*/
        (*knot)->parent = *knot;
    }
}

/*
 * function @tree_fnd_object finds an object in the tree and 
 * returns a pointer to this object
 */
void* tree_fnd_object(tree_t *t, void *o)
{
    /*pointer to pointer to tree root*/
    knot_t **knot = &t->knot;

    /*traversing a tree with a pointer to pointer in a loop */
    while(*knot != NULL)
    {
        /*if the object being added is less than the current one, 
         * go to the left subtree*/
        if(t->compare_fn((*knot)->data, o) < 0) 
        {
            /*move to left subtree*/
            knot = &(*knot)->left;
        }
        /*if the object being added is greater than the current one, 
         * go to the right subtree*/
        else if(t->compare_fn((*knot)->data, o) > 0)
        {
            /*move to right subtree*/
            knot = &(*knot)->right;
        }
        /*if the objects are equal then forced exit from the loop*/
        else if(t->compare_fn((*knot)->data, o) == 0)
        {
            /*forced exit from the loop*/
            break;
        }
    }

    /*returning a pointer to an object*/
    return (void *)(*knot)->data;
}

/*
 * function @tree_rmv_object to remove an object from the tree
 */
void* tree_rmv_object(tree_t *t, void *o)
{
    /*pointer to pointer to tree root*/
    knot_t **knot = &t->knot;

    /*traversing a tree with a pointer to pointer in a loop */
    while(*knot != NULL)
    {
        /*if the object being added is less than the current one, 
         * go to the left subtree*/
        if(t->compare_fn((*knot)->data, o) < 0) 
        {
            /*move to left subtree*/
            knot = &(*knot)->left;
        }

        /*if the object being added is greater than the current one, 
         * go to the right subtree*/
        else if(t->compare_fn((*knot)->data, o) > 0)
        {
            /*move to right subtree*/
            knot = &(*knot)->right;
        }

        /*if the objects are equal then forced exit from the loop*/
        else if(t->compare_fn((*knot)->data, o) == 0)
        {
            /*when a knot has no sons*/
            if((*knot)->left == NULL && (*knot)->right == NULL)
            {
                /*pointer to the object to be retrieved from the knot 
                 * to be removed*/
                void *data_of_deleting_knot = (*knot)->data;

                /*removing a knot from a tree*/
                t->iallocator->deallocate(NULL, *knot);

                /*knot pointer is now NULL*/
                *knot = NULL;

                /*return a pointer to the object that was in the tree knot 
                 * being deleted*/
                return data_of_deleting_knot;
            }
            
            /*when there is a left son and no right*/
            else if((*knot)->left != NULL && (*knot)->right == NULL)
            {
                /*pointer to the object to be retrieved from the knot 
                 * to be removed*/
                void *data_of_deleting_knot = (*knot)->data;

                /*another pointer to the knot to be removed*/
                knot_t* tmp = *knot;

                /*the pointer now points to the left subtree relative 
                 * to the knot being removed*/
                *knot = (*knot)->left;

                /*now the root in the left subtree stores a pointer 
                 * to the parent of the knot being deleted*/
                (*knot)->parent = tmp->parent;

                /*deleting a knot*/
                t->iallocator->deallocate(NULL, tmp);

                /*pointer to remote knot is now NULL*/
                tmp = NULL;

                /*return a pointer to the object that was in 
                 * the tree knot being deleted*/
                return data_of_deleting_knot;
            }
            
            /*when there is a right son and no left*/
            else if((*knot)->right != NULL && (*knot)->left == NULL)
            {
                /*pointer to the object to be retrieved from the knot 
                 * to be removed*/
                void *data_of_deleting_knot = (*knot)->data;

                /*another pointer to the knot to be removed*/
                knot_t* tmp = *knot;

                /*the pointer now points to the right subtree relative 
                 * to the knot being removed*/
                *knot = (*knot)->right;

                /*now the root in the left subtree stores a pointer 
                 * to the parent of the knot being deleted*/
                (*knot)->parent = tmp->parent;

                /*deleting a knot*/
                t->iallocator->deallocate(NULL, tmp);

                /*pointer to remote knot is now NULL*/
                tmp = NULL;

                /*return a pointer to the object that was in 
                 * the tree knot being deleted*/
                return data_of_deleting_knot;
            }

            /*when a knot has two sons*/
            else if((*knot)->left != NULL && (*knot)->right != NULL)
            {
                /*pointer to the right subtree pointer of the knot to be deleted*/
                knot_t **tmp = &(*knot)->right; 
                
                /*traversal of the right subtree from the knot to 
                 * be removed to the minimum element*/
                while((*tmp)->left != NULL)
                {
                    /*traversal to the leftmost knot of the subtree*/
                    tmp = &(*tmp)->left;
                }
                
                /*pointer to the object to be retrieved from the knot 
                 * to be removed*/
                void *data_of_deleting_knot = (*knot)->data;

                /*we put data from the leftmost knot in the subtree 
                 * into the knot to be deleted*/
                (*knot)->data = (*tmp)->data;

                /*put the data from the knot to be deleted into the 
                 * leftmost knot of the subtree*/
                (*tmp)->data = data_of_deleting_knot;

                /*create a pointer to the leftmost knot in the subtree, 
                 * with the help of which will later delete this knot*/
                knot_t *deleting_knot = *tmp;

                /*if the leftmost knot of the right subtree of the knot 
                 * to be removed has a right subtree*/
                if((*tmp)->right != NULL)
                {
                    /*now point to the left subtree of the leftmost knot*/
                    *tmp = (*tmp)->right;

                    /*at the root of the right subtree, the parent is 
                     * now the knot that is the parent of the knot being deleted*/
                    (*tmp)->parent = deleting_knot->parent;

                    /*deleting a knot*/
                    t->iallocator->deallocate(NULL, deleting_knot);

                    /*pointer to remote knot is now NULL*/
                    deleting_knot = NULL;

                    /*return a pointer to the object that was in the 
                     * tree knot being deleted*/
                    return data_of_deleting_knot;
                }

                /*if the leftmost knot of the right subtree of the knot to 
                 * be removed has not a right subtree*/
                if((*tmp)->right == NULL) 
                {
                    /*deleting a knot*/
                    t->iallocator->deallocate(NULL, *tmp);

                    /*pointer to remote knot is now NULL*/
                    *tmp = NULL;

                    /*return a pointer to the object that was in 
                     * the tree knot being deleted*/
                    return data_of_deleting_knot;
                }
            }
        }
    }

    /*return NULL if the object to be removed was not found*/
    return NULL;
}

void tree_print(tree_t *t, void (*print_fn)(void *o))
{
    if(t != NULL)
        knot_print(t->knot, print_fn, 0);
}

/*
 * function @knot_create creates a knot and returns a pointer to the created knot
 */
static knot_t *knot_create(void *data, 
                           iallocator_t *iallocator, 
                           knot_t *parent)
{
    /*creating a pointer to the knot to be returned*/
    knot_t *knot = (knot_t *)iallocator->allocate(iallocator, 
                                                  sizeof(knot_t));
    
    /*initializing the fields of the knot structure with an object, 
     * pointers to the parent knot and the knots of the sons*/
    knot->data = data;
    knot->left = knot->right = NULL;
    knot->parent = parent;

    /*return a pointer to the knot being created*/
    return knot;
}

/**/
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

/*
 * function @tree_delete_all_knots to delete all knots in the tree using recursion
 */
static void tree_delete_all_knots(knot_t **knot, 
                                  void (*dealloc_fn)(void *o), 
                                  void (*deallocate)(void *self, void *address))
{
    /*while the pointer points to an existing knot in the tree*/
    if(*knot != NULL)
    {
        /*move to left subtree*/
        tree_delete_all_knots(&(*knot)->left, dealloc_fn, deallocate);

        /*move to right subtree*/
        tree_delete_all_knots(&(*knot)->right, dealloc_fn, deallocate);

        /*deleting an object from a knot*/
        dealloc_fn((*knot)->data);

        /*removing a knot from a tree*/
        deallocate(NULL, *knot);
    }
}

/*
 * function @copy_fake returns a pointer to an object
 */
static void *copy_fake(void *o) 
{ 
    /*returning a pointer to an object*/
    return o; 
}

/*
 * function @bst_to_vine convert tree to right singly linked list
 */
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
    knot_t *grand = knot_create(NULL, t->iallocator, NULL); 
    grand->right = t->knot;
    int count = bst_to_vine(grand);
    int h = log2((double)(count + 1));
    int m = pow(2, h) - 1;
    compress(grand, count - m);
    for(m = m / 2; m > 0; m /= 2)
        compress(grand, m);
    knot_t *result = grand->right;
    t->iallocator->deallocate(NULL, grand);
    return result;
}

void tree_balance_DSW(tree_t *t) 
{ 
    t->knot = (knot_t *)tree_balance(t); 
}
