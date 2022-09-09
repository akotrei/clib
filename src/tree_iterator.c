#include "allocator_std.h"
#include "tree_type_public.h"
#include "tree_type_private.h"
#include "tree_iterator.h"

#include <assert.h>
#include <stdlib.h>

/*
 * @tree_get_data function to receive data from the knot
 *
 * @knot    - pointer to the knot
 */
void*
get_data(void *knot)
{
    assert(knot != NULL && "@knot mustn't be NULL");
    return ((knot_t *)knot)->data;
}

void*
curr(void *t)
{
    assert(t != NULL && "@t mustn't be NULL");
    return ((tree_t *)t)->curr;
}

/*
 * @tree_reset function that resets the current knot to the root knot
 */
void
reset(void *t)
{
    assert(t != NULL && "@t mustn't be NULL");
    ((tree_t *)t)->curr = ((tree_t *)t)->root;
}

/*
 * @left function to move the left subtree
 */
void
left(void *t)
{
    assert(t != NULL && "@t mustn't be NULL");
    ((tree_t *)t)->curr = ((tree_t *)t)->curr->left;
}

/*
 * @right function to move the right subtree
 */
void
right(void *t)
{
    assert(t != NULL && "@t mustn't be NULL");
    ((tree_t *)t)->curr = ((tree_t *)t)->curr->right;
}

/*
 * @parent function to move the previous knot
 */
void
parent(void *t)
{
    assert(t != NULL && "@t mustn't be NULL");
    ((tree_t *)t)->curr = ((tree_t *)t)->curr->parent;
}

itree_iterator_t*
tree_iterator_create(void *mem)
{
    assert(mem != NULL && "@mem mustn't be NULL");
    itree_iterator_t *itree_iterator = (itree_iterator_t *)mem;
    itree_iterator->reset = reset;
    itree_iterator->get_data = get_data;
    itree_iterator->left = left;
    itree_iterator->right = right;
    itree_iterator->parent = parent;
    itree_iterator->curr = curr;
    return itree_iterator;
}
