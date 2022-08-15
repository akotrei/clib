#include "array.h"
#include "allocator_std.h"
#include "array_type_private.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* function @dealloc_fake object removal function @dealloc_fn if equal NULL
 *
 * @o    - pointer to object
 */
inline static void dealloc_fake(void *o) {}

/* function @copy_fake returns pointer to object if @copy_fn is equal NULL
 *
 * @o    - pointer to obejct
 */
inline static void* copy_fake(void *o) { return o; }

/* @array_increase_capacity function increases the memory for the array by @factor value
 *
 * @a      - pointer to array
 *
 * @factor - memory increase indicator, that is, the number by which 
 *           memory for the array will increase
 */
static void 
array_increase_capacity(array_t *a, 
                        int factor);

array_t* 
array_create(int alloc_size,
             int elem_size,
             void* (*copy_fn)(void *o),
             void (*dealloc_fn)(void *o),
             int (*compare_fn)(const void *o1, const void *o2),
             int factor,
             iallocator_t *iallocator)
{
    /*pointer to allocator interface*/
    iallocator_t *_iallocator;

    /*pointer to standard allocator*/
    allocator_std_t *iallocator_std = NULL;

    /*variable that will be needed for initialization @iallocator_owner*/
    int _iallocator_owner;

    /* if the pointer to the allocator interface passed to
     * the function creating a list is NULL then a standard allocator is created
     */
    if(iallocator == NULL)
    {
        /*creating a standard allocator*/
        iallocator_std = allocator_std_new();

        /*getting interface allocator from standard allocator*/
        _iallocator = allocator_std_get_allocator(iallocator_std);

        /*now when the list is deleted, the allocator will be deleted*/
        _iallocator_owner = 1;
    }
    /*if the pointer to the allocator interface passed
     * to the function creating a list is not NULL
     */
    else
    {
        /*allocator interface assignment*/
        _iallocator = iallocator;

        /*deleting the allocator when deleting the list will not be done*/
        _iallocator_owner = 0;
    }
    
    array_t *a = (array_t *)_iallocator->allocate(_iallocator->self, 
                                                  sizeof(array_t));

    a->factor = factor;
    a->iallocator = _iallocator;
    a->alloc_size = alloc_size;
    a->logic_size = 0;
    a->elem_size = elem_size;
    a->data = (void *)a->iallocator->allocate(a->iallocator, 
                                              alloc_size * elem_size);
    a->compare_fn = compare_fn;

    if(dealloc_fn != NULL)
        a->dealloc_fn = dealloc_fn;
    else 
        a->dealloc_fn = dealloc_fake;

    if(copy_fn != NULL)
        a->copy_fn = copy_fn;
    else 
        a->copy_fn = copy_fake;

    return a;
}

void 
array_delete(array_t *a)
{
    void (*deallocate)(void *self, void *addr) = a->iallocator->deallocate;
    a->dealloc_fn(a->data);
    //if(a->iallocator_owner == 1)
    //{
        /*allocator removal*/
        allocator_std_delete(a->iallocator->self);
    //}

    /*deleting a list*/
    deallocate(NULL, a);
}

void 
array_push_back(array_t *a, 
                void *obj)
{
    if(a->alloc_size == a->logic_size)
    {
        array_increase_capacity(a, a->factor);
    }
	a->iallocator->copy_data(NULL, (char *)a->data + a->logic_size * a->elem_size, a->copy_fn(obj), a->elem_size);   
	a->logic_size++;
}

void
array_insert(array_t *a,
             void *obj,
             int index,
             int count)
{
    if(a->alloc_size - a->logic_size <= count)
    {
    }
}

void*
array_get_data(array_t *a,
               int index)
{
    return (void *)((char *)a->data + index * a->elem_size);
}

int
array_get_index(array_t *a,
                void *obj)
{
    
}

void*
array_rmv_element(array_t *a,
                  int index)
{
}

static void 
array_increase_capacity(array_t *a, 
                        int factor)
{
    a->alloc_size *= factor;
    a->data = a->iallocator->reallocate(NULL, a->data, a->alloc_size * a->elem_size);
}
