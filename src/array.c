#include "array.h"
#include "allocator_std.h"
#include "array_type_private.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

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
array_create(void *buffer,
             int alloc_size,
             int elem_size,
             void* (*copy_fn)(void *o),
             void (*dealloc_fn)(void *o),
             int (*compare_fn)(const void *o1, const void *o2),
             int factor,
             iallocator_t *iallocator)
{
    assert(alloc_size > 0 && "@alloc_size must be > 0");
    assert(elem_size > 0 && "@elem_size must be > 0");
    assert(factor > 1 && "@factor must be > 1");
    assert(buffer != NULL && "@buffer mustn't be NULL");
    assert(iallocator != NULL && "@iallocator mustn't be NULL");

    /* create array*/
    array_t *a = (array_t *)buffer;

    /* initialization field of array structure*/
    if(factor == 0)
        a->factor = 2;
    else
        a->factor = factor;

    a->iallocator = iallocator;
    a->alloc_size = alloc_size;
    a->logic_size = 0;
    a->elem_size = elem_size;

    /* allocating memory for array data*/
    a->data = (void *)a->iallocator->allocate(NULL,
                                              alloc_size * elem_size);
    /* initialization*/
    a->compare_fn = compare_fn;

    if(dealloc_fn != NULL)
        a->dealloc_fn = dealloc_fn;
    else
        a->dealloc_fn = dealloc_fake;

    if(copy_fn != NULL)
        a->copy_fn = copy_fn;
    else
        a->copy_fn = copy_fake;

    /* returning a pointer to created array*/
    return a;
}

/*
 * @array_sizeof function returns size in the bytes structure of the array
 */
int
array_sizeof()
{
    return sizeof(array_t);
}

void
array_delete(array_t *a)
{
    int elem_size = a->elem_size;
    char* data = (char*)a->data;
    void (*dealloc_fn)(void*) = a->dealloc_fn;
    iallocator_t* iallocator = a->iallocator;

    /* deleting objects in an array*/
    for (int i = 0; i < a->logic_size; i++)
    {
        void* obj = data + i * elem_size;
        dealloc_fn(obj);
    }

    iallocator->deallocate(NULL, data);
    iallocator->deallocate(NULL, a);
}

void
array_push_back(array_t *a,
                void *obj)
{
    /* if the size of the memory allocated for the array is equal to the size
     * occupied by the memory objects, then we re-allocate the memory for
     * the @factor parameter*/
    if(a->alloc_size == a->logic_size)
    {
        array_increase_capacity(a, a->factor);
    }

    /* copy object to array*/
	a->iallocator->replicate(NULL,
                             (char *)a->data + a->logic_size * a->elem_size,
                             a->copy_fn(obj), a->elem_size);
    /* increasing the size of an array*/
	a->logic_size++;
}

void
array_insert(array_t *a,
             void *obj,
             int index,
             int count)
{
    assert(index >= 0 && "@index mustn't be < 0");
    assert(count > 0 && "@count mustn't be < 1");

    int alloc_size = a->alloc_size;
    int logic_size = a->logic_size;
    int elem_size = a->elem_size;
    int result_size = logic_size + count;
    /* if there is not enough allocated memory for adding objects to the array*/
    if(alloc_size < result_size)
    {
        float factor_f = ((float)result_size) / ((float)alloc_size) + 1.0f;
        int factor = (int)factor_f;
        array_increase_capacity(a, factor);
    }

    /* if the index is the end of the array*/
    if(index >= logic_size)
    {
        a->iallocator->replicate(NULL,
                                 (char *)a->data + logic_size * elem_size,
                                 a->copy_fn(obj), elem_size * count);
        a->logic_size += count;
    }
    /* otherwise we shift the data to the right to add objects*/
    else
    {
        /* copy the data byte by byte to the right side of the array*/
        char *begin_moving_data = (char *)a->data + index * elem_size;
        char *end_moving_data = (char *)a->data + logic_size * elem_size - 1;
        char *new_place_data = (char *)a->data + (logic_size + count) * elem_size - 1;
        while(end_moving_data != begin_moving_data)
        {
            *new_place_data-- = *end_moving_data--;
        }
        a->iallocator->replicate(NULL,
                                 begin_moving_data, a->copy_fn(obj),
                                 elem_size * count);
        a->logic_size += count;
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
    assert(a->compare_fn != NULL && "@compare_fn mustn't be NULL");

    int index = 0;
    char *tmp = (char *)a->data;
    int elem_size = a->elem_size;
    while(a->compare_fn((const void *)tmp, (const void *)obj) != 0)
    {
        tmp = tmp + elem_size;
        index++;
    }
    return index;
}

void array_factor(array_t *a, int factor) { a->factor = factor; }

int array_size(array_t *a) { return a->logic_size; }

void
array_rmv_element(array_t *a,
                  int index)
{
    assert(a->compare_fn != NULL && "@compare_fn mustn't be NULL");
    assert(index >= 0 && "@index must be > 0");

    if(index >= a->logic_size)
    {
        a->logic_size--;
    }
    else
    {
        char *tmp = (char *)a->data + index * a->elem_size;
        a->dealloc_fn(tmp);
        char *tmp1 = (char *)a->data + (index + 1) * a->elem_size;

        while(tmp1 != (char *)a->data + a->logic_size * a->elem_size)
        {
            *tmp++ = *tmp1++;
        }
        a->logic_size--;
    }
}

static void
array_increase_capacity(array_t *a,
                        int factor)
{
    a->alloc_size *= factor;
    a->data = a->iallocator->reallocate(NULL, a->data, a->alloc_size * a->elem_size);
}
