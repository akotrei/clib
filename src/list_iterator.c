#include "allocator_std.h"
#include "list_type_public.h"
#include "list_type_private.h"
#include "list_iterator.h"

#include <assert.h>
#include <stdlib.h>

static void
_begin(void *l)
{
    ((list_t *)l)->curr = ((list_t *)l)->head;
}

static void
_end(void *l)
{
    ((list_t *)l)->curr = ((list_t *)l)->tail;
}

static void*
_get_data(void *node)
{
    return (void *)((node_t *)node)->data;
}

static void*
_curr(void *l)
{
    return ((list_t *)l)->curr;
}

static void
_next(void *l)
{
    ((list_t *)l)->curr = ((list_t *)l)->curr->next;
}

static void
_prev(void *l)
{
    ((list_t *)l)->curr = ((list_t *)l)->curr->prev;
}

/*
 * Function of creating the tree iterator
 */
ilist_iterator_t *
list_iterator_create(void *mem)
{
    ilist_iterator_t *ilist_iterator = (ilist_iterator_t *)mem;
    ilist_iterator->begin = _begin;
    ilist_iterator->end = _end;
    ilist_iterator->get_data = _get_data;
    ilist_iterator->curr = _curr;
    ilist_iterator->next = _next;
    ilist_iterator->prev = _prev;
    return ilist_iterator;
}
