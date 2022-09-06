#include "tree.h"
#include "allocator_std.h"
#include "tree_type_private.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

/*
 * knot creation function @knot_create that returns a pointer to the created knot
 *
 * @data                   - pointer to user object
 *
 * @interface_allocator    - pointer to allocator interface
 *
 * @parent                 - pointer to previous knot
 */
static knot_t*
knot_create(void *data,
            void *mem,
            knot_t *parent);

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
static void
tree_delete_all_knots(knot_t **knot,
                      void (*dealloc_fn)(void *o),
                      void (*deallocate)(void *self, void *address));

/*
 * a function @copy_fake that returns a pointer to the user object if @copy_fn
 * is equal NULL
 *
 * @o    - pointer to user object
 */
static void* copy_fake(void *o) { return o; }

/* a function that does not delete anything if @dealloc_fn is NULL
 *
 * @o    - pointer to user object
 */
static void dealloc_fake(void *o) {}

/*
 * function @tree_create creates a tree and returns a pointer to it
 */
tree_t*
tree_create(void *buffer,
            int (*compare_fn)(void *o1, void *o2),
            void* (*copy_fn)(void *o),
            void (*dealloc_fn)(void *o),
            iallocator_t *iallocator)
{
    assert(compare_fn != NULL && "@compare_fn mustn't be NULL");
    assert(buffer != NULL && "@buffer mustn't be NULL");
    assert(iallocator != NULL && "@iallocator mustn't be NULL");

    /*creating a tree using the allocator interface*/
    tree_t *t = (tree_t *)buffer;

    /*tree structure field assignment*/
    t->iallocator = iallocator;
    t->compare_fn = compare_fn;
    t->root = NULL;
    t->curr = NULL;

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
void
tree_delete(tree_t *t)
{
    assert(t->iallocator != NULL && "@iallocator mustn't be NULL");

    /*pointer to a function that removes the tree itself*/
    void (*deallocate)(void *self,
                       void *address) = t->iallocator->deallocate;

    /*a function that recursively removes all knots of a tree*/
    tree_delete_all_knots(&t->root, t->dealloc_fn, deallocate);

    /*tree removal*/
    deallocate(NULL, t);
}

int
tree_sizeof() { return sizeof(tree_t); }

/*
 * function @tree_add_object to add an object to the tree
 */
void tree_add_object(tree_t *t, void *o)
{
    assert(t->compare_fn != NULL && "@compare_fn mustn't be NULL");
    assert(t->iallocator != NULL && "@iallocator mustn't be NULL");

    /*pointer to pointer to tree root*/
    knot_t **knot = &t->root;

    /*pointer to previous knot*/
    knot_t *tmp_parent = NULL;

    /*traversing a tree with a pointer to pointer in a loop */
    while(*knot != NULL)
    {
        /*pointer to the previous knot in the tree,
         * this is necessary in order to save this pointer in the created knot*/
        tmp_parent = *knot;

        /*if the object being added is less than the current one,
         * go to the left subtree*/
        if (t->compare_fn((*knot)->data, o) < 0)
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
        /*if the objects are equal then exit the function*/
        else if(t->compare_fn((*knot)->data, o) == 0)
        {
            /*function exit*/
            return;
        }

    }

    /*created knot*/
    *knot = knot_create(t->copy_fn(o),
                        t->iallocator->allocate(NULL, sizeof(knot_t)),
                        tmp_parent);

    if(t->curr == NULL)
        t->curr = *knot;
}

/*
 * function @tree_fnd_object finds an object in the tree and
 * returns a pointer to this object
 */
void*
tree_fnd_object(tree_t *t,
                void *o)
{
    assert(t->compare_fn != NULL && "@compare_fn mustn't be NULL");

    /*pointer to pointer to tree root*/
    knot_t **knot = &t->root;

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
void*
tree_rmv_object(tree_t *t,
                void *o)
{
    assert(t->compare_fn != NULL && "@compare_fn mustn't be NULL");
    assert(t->iallocator != NULL && "@iallocator mustn't be NULL");

    /*pointer to pointer to tree root*/
    knot_t **knot = &t->root;

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

/*
 * function @knot_create creates a knot and returns a pointer to the created knot
 */
static knot_t*
knot_create(void *data,
            void *mem,
            knot_t *parent)
{
    /*creating a pointer to the knot to be returned*/
    knot_t *knot = (knot_t *)mem;

    /*initializing the fields of the knot structure with an object,
     * pointers to the parent knot and the knots of the sons*/
    knot->data = data;
    knot->left = knot->right = NULL;
    knot->parent = parent;

    /*return a pointer to the knot being created*/
    return knot;
}

/*
 * function @tree_delete_all_knots to delete all knots in the tree using recursion
 */
static void
tree_delete_all_knots(knot_t **knot,
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
 * function @bst_to_vine convert tree to right singly linked list
 */
static int
bst_to_vine(knot_t *grand)
{
    int count = 0;

    /* make tmp pointer to traverse
     * and right flatten the given BST*/
    knot_t *tmp = grand->right;

    /* traverse until tmp becomes NULL*/
    while(tmp != NULL)
    {
        /* if left exist for node
         * pointed by tmp then
         * right rotate it */
        if(tmp->left != NULL)
        {
            knot_t *oldtmp = tmp;
            tmp = tmp->left;
            oldtmp->left = tmp->right;
            tmp->right = oldtmp;
            grand->right = tmp;
        }
        /* if left dont exists
         * add 1 to count and
         * traverse further right to
         * flatten remaining BST */
        else
        {
            count++;
            grand = tmp;
            tmp = tmp->right;
        }
    }
    return count;
}

/* Function to compress given tree
 * with its root as grand->right.
 */
static void
compress(knot_t *grand,
         int m)
{
    /* make tmp pointer to traverse
     * and right flatten the given BST*/
    knot_t *tmp = grand->right;

    /* Traverse and left-rotate root m times
     * to compress given vine form of BST */
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

/* function to implement the algorithm*/
static knot_t*
tree_balance(tree_t *t)
{
    assert(t->iallocator != NULL && "@iallocator mustn't be NULL");

    /* create dummy node*/
    knot_t *grand = knot_create(NULL,
                                t->iallocator->allocate(NULL, sizeof(knot_t)),
                                NULL);

    /* assign the right of dummy node as our input BST*/
    grand->right = t->root;

    /* get the number of nodes in input BST and
     * simultaneously convert it into right linked list.*/
    int count = bst_to_vine(grand);

    /* gets the height of tree in which all levels
     * are completely filled.*/
    int h = log2((double)(count + 1));

    /* get number of nodes until second last level*/
    int m = pow(2, h) - 1;

    /* left rotate for excess nodes at last level*/
    compress(grand, count - m);

    /* left rotation till m becomes 0
     * step is done as mentioned in algo to
     * make BST balanced */
    for(m = m / 2; m > 0; m /= 2)
        compress(grand, m);

    knot_t *result = grand->right;

    /* deleting the created temporary node*/
    t->iallocator->deallocate(NULL, grand);

    /* return the balanced tree*/
    return result;
}

void
tree_balance_DSW(tree_t *t)
{
    /* calling function balancing of the tree using DSW algorithm*/
    t->root = t->curr = (knot_t *)tree_balance(t);
}
