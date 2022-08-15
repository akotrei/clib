#include "array.h"
#include "allocator_std.h"
#include "array_type_private.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* function @dealloc_fake 
 *
 * @o    - pointer to object
 */
inline static void dealloc_fake(void *o) {};

/* function @copy_fake 
 *
 * @o    - pointer to obejct
 */
inline static void* copy_fake(void *o) { return o; };

static void array_increase_capacity(array_t *a, int factor);

array_t *array_create(int factor, 
                      int alloc_size,
                      int elem_size,
                      void* (*copy_fn)(void *o),
                      void (*dealloc_fn)(void *o),
                      int (*compare_fn)(void *o1, void *o2),
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
    a->iallocator_owner = _iallocator_owner;
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

void array_delete(array_t *a)
{
    int elem_size = a->elem_size;
    char* data = (char*)a->data;
    void (*dealloc_fn)(void*) = a->dealloc_fn;
    iallocator_t* iallocator = a->iallocator;

    for (int i = 0; i < a->logic_size; i++)
    {
        void* obj = data + i * elem_size;
        dealloc_fn(obj);
    }

    iallocator->deallocate(NULL, data);

    if (a->iallocator_owner == 1)
    {
        iallocator->deallocate(NULL, a);
        allocator_std_delete(iallocator->self);
    }
    else
    {
        iallocator->deallocate(NULL, a);
    }
}

void array_push_back(array_t *a, void *obj)
{
    if(a->alloc_size == a->logic_size)
    {
        array_increase_capacity(a, a->factor);
    }
	a->iallocator->copy_data(NULL, (char *)a->data + a->logic_size * a->elem_size, a->copy_fn(obj), a->elem_size);   
	a->logic_size++;
}

void array_push_front(array_t *a, void *obj)
{
    if(a->alloc_size == a->logic_size)
    {
        array_increase_capacity(a, a->factor);
    }
	int i = a->logic_size * a->elem_size;
	while(i != 0)
	{
		char *pointer_ending_data = (char *)a->data + i - 1;
		char *pointer_free_store_for_object = (char *)a->data + i + a->elem_size - 1;
		*pointer_free_store_for_object = *pointer_ending_data;
		i--;
	}
	a->iallocator->copy_data(NULL, (char *)a->data, a->copy_fn(obj), a->elem_size);   
	a->logic_size++;
}

void array_insert_obj_by_index(array_t *a, void *obj, int index)
{
    if(a->alloc_size == a->logic_size)
    {
        array_increase_capacity(a, a->factor);
    }
	
}

void* array_fnd_obj(array_t *a, void *obj)
{
    char *tmp = (char *)a->data;
    char *tmp_obj = (char *)obj;
    while(tmp != tmp_obj)
    {
        tmp += a->elem_size;
    }
    return tmp;
}

void* array_rmv_obj(array_t *a, void *obj)
{
	return NULL;
}

void array_print(array_t *a, void (*print_fn)(void *o))
{
    char *tmp = (char *)a->data;
    int i = 0;
    while(i < a->logic_size)
    {
        print_fn((void *)tmp);
        tmp += a->elem_size;
        i++;
    }
    printf("\n");
}

static void array_increase_capacity(array_t *a, int factor)
{
    a->alloc_size *= factor;
    a->data = a->iallocator->reallocate(NULL, a->data, a->alloc_size * a->elem_size);
}
