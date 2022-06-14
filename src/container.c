#include "container.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

container *container_new(int alloc_size, int elem_size, void (*dealloc_fn)(void *))
{
    assert(alloc_size > 0 && "@alloc_size must be more than 0");
    assert(elem_size > 0 && "@elem_size must be more than 0");

    container *c = NULL;
    c = (container *)malloc(sizeof(container));
    assert(c != NULL && "memory for @container hasn't been allocated");

    c->alloc_size = alloc_size;
    c->logic_size = 0;
    c->elem_size = elem_size;
    c->dealloc_fn = dealloc_fn;
    c->is_wrapper = 0;
    c->data = malloc(alloc_size * elem_size);
    assert(c->data != NULL && "memory for @containers data hasn't been allocated");

    return c;
}

container *container_new_as_warapper(int size, int elem_size, void *data)
{
    assert(size > 0 && "@size must be more than 0");
    assert(elem_size > 0 && "@elem_size must be more than 0");

    container *c = NULL;
    c = (container *)malloc(sizeof(container));
    assert(c != NULL && "memory for @container hasn't been allocated");

    c->alloc_size = size;
    c->logic_size = size;
    c->elem_size = elem_size;
    c->data = data;
    c->dealloc_fn = NULL;
    c->is_wrapper = 1;

    return c;
}

void container_delete(container *c)
{
    // if @dealloc_fn is not NULL
    if (c->dealloc_fn)
    {
        char *elem;
        for (int i = 0; i < c->logic_size; i++)
        {
            elem = (char *)(c->data) + i * c->elem_size;
            c->dealloc_fn(elem);
        }
    }
    if (!c->is_wrapper)
    {
        free(c->data);
    }
    free(c);
}

void container_add_element(container *c, void *element)
{
    assert(c->is_wrapper == 0 && "this container is a wrapper");

    if (c->alloc_size == c->logic_size)
    {
        container_increase_capacity(c, 2);
    }
    memcpy((char *)c->data + c->logic_size * c->elem_size, element, c->elem_size);
    c->logic_size++;
}

void container_add_elements(container *c, void *elements, int count)
{
    assert(c->is_wrapper == 0 && "this container is a wrapper");
    assert(count > 0 && "@count must be more than 0");

    int result_size = c->logic_size + count;
    int alloc_size = c->alloc_size;
    if (alloc_size < result_size)
    {
        float factor_f = ((float)result_size) / ((float)alloc_size) + 1.0f;
        int factor = (int)factor_f;
        container_increase_capacity(c, factor);
    }
    memcpy((char *)c->data + c->logic_size * c->elem_size, elements, c->elem_size * count);
    c->logic_size += count;
}

void *container_remove_last_element(container *c)
{
    assert(c->is_wrapper == 0 && "this container is a wrapper");

    if (c->logic_size == 0)
    {
        return NULL;
    }

    c->logic_size--;
    char *last_elem = (char *)c->data + c->logic_size * c->elem_size;

    return (void *)last_elem;
}

void *container_remove_last_elements(container *c, int count)
{
    assert(c->is_wrapper == 0 && "this container is a wrapper");

    if (c->logic_size < count)
    {
        return NULL;
    }

    c->logic_size -= count;
    char *last_elems = (char *)c->data + c->logic_size * c->elem_size;

    return (void *)last_elems;
}

void container_increase_capacity(container *c, int factor)
{
    assert(c->is_wrapper == 0 && "this container is a wrapper");
    assert(factor > 1 && "@factor mast be more than 1");

    c->alloc_size *= factor;
    c->data = realloc(c->data, c->alloc_size * c->elem_size);
    assert(c->data != NULL && "memory for @containers data hasn't been reallocated");
}

void *container_get_element(container *c, int index)
{
    assert((index >= 0) && (index < c->logic_size) && "index out of range");
    char *elem = (char *)c->data + c->elem_size * index;
    return elem;
}

void container_dealloc_as_pointer(void *c)
{
    container_delete(*(container **)c);
}