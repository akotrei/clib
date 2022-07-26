#include "list.h"
#include "allocator_std.h"

#include <stdlib.h>
#include <stdio.h>

/* function @dealloc_fake doesn't delete anything if NULL is passed when 
 * creating the list instead of the object deletion function
 *
 * @o    - pointer to object
 */
inline static void dealloc_fake(void *o) {};

/* a function that returns a pointer to an object if zero was passed 
 * when creating the list instead of the object copy function
 *
 * @o    - pointer to obejct
 */
inline static void* copy_fake(void *o) { return o; };

/* the function creates a node in the list and returns a pointer 
 * to the newly created node
 *
 * @data          - pointer to data in node
 *
 * @iallocator    - pointer to allocator interface
 *
 * @next          - pointer to the next node in the list
 *
 * @prev          - pointer to the previous node in the list
 */
static node_t* node_create(void *data, iallocator_t *iallocator, 
                           node_t *next, node_t *prev);

//static void node_delete(node_t* n, void (*dealloc_fn)(void*));

list_t* list_create(void* (*copy_fn)(void *obj),
                    void (*dealloc_fn)(void *obj),
                    int (*compare_fn)(void *obj1, void *obj2),
                    iallocator_t* iallocator)
{
    iallocator_t *_iallocator;
    
    allocator_std_t *iallocator_std = NULL;

    int _iallocator_owner;

    if(iallocator == NULL)
    {
        iallocator_std = allocator_std_new();

        _iallocator = allocator_std_get_allocator(iallocator_std);

        _iallocator_owner = 1;
    }
    else 
    {
        _iallocator = iallocator;
        _iallocator_owner = 0;
    }

    list_t *l = (list_t *)_iallocator->allocate(_iallocator->self, sizeof(list_t));

    l->iallocator = _iallocator;
    l->iallocator_owner = _iallocator_owner;

    l->compare_fn = compare_fn;
    l->head = l->tail = NULL;

    if(dealloc_fn != NULL)
        l->dealloc_fn = dealloc_fn;
    else
        l->dealloc_fn = dealloc_fake;

    if(copy_fn != NULL)
        l->copy_fn = copy_fn;
    else
        l->copy_fn = copy_fake;

    return l;
}

void list_delete(list_t* list)
{
    node_t *node = list->head;
    void (*deallocate)(void *self, void *address) = list->iallocator->deallocate;
    while(node != NULL)
    {
        list->dealloc_fn(node->data);
        node_t *tmp = node;
        node = node->next;
        deallocate(NULL, tmp);
    }

    if(list->iallocator_owner == 1)
    {
        allocator_std_delete(list->iallocator->self);
    }

    deallocate(NULL, list);
}

void list_add_head(list_t *list, void *obj)
{
    if(list->head == NULL)
    {
        list->head = list->tail = (node_t *)node_create(list->copy_fn(obj), 
                                                        list->iallocator, NULL, 
                                                        NULL);
    }
    else 
    {
        node_t *tmp = list->head;
        node_t *node = (node_t *)node_create(list->copy_fn(obj), list->iallocator
                                             , tmp, NULL);
        list->head->prev = node;
        list->head = node;
    }
}

void list_add_tail(list_t* list, void* obj)
{
    if(list->tail == NULL)
    {
        list->head = list->tail = (node_t *)node_create(list->copy_fn(obj), 
                                                        list->iallocator, NULL, 
                                                        NULL);
    }
    else 
    {
        node_t *tmp = list->tail;
        node_t *node = (node_t *)node_create(list->copy_fn(obj), list->iallocator
                                             , NULL, tmp);
        list->tail->next = node;
        list->tail = node;
    }
}

void list_prt_shead(list_t *list, void (*print_fn)(void *o))
{
    node_t **tmp = &list->head;
    while(*tmp != NULL)
    {
        print_fn((*tmp)->data);
        tmp = &(*tmp)->next;
    }
}

void list_prt_stail(list_t *list, void (*print_fn)(void *o))
{
    node_t **tmp = &list->tail;
    while(*tmp != NULL)
    {
        print_fn((*tmp)->data);
        tmp = &(*tmp)->prev;
    }
}

static node_t* node_create(void *data, iallocator_t *iallocator, 
                           node_t *next, node_t *prev)
{
    node_t *node = (node_t *)iallocator->allocate(iallocator, sizeof(node_t));
    node->data = data;
    node->next = next;
    node->prev = prev;
    return node;
}
