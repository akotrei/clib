#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "types.h"

#include <stddef.h>

// create new container
// return @container pointer to created container
// @alloc_size - initial allocated size in @elem_size bytes
// @elem_size - size of one element in bytes
// @dealloc_fn - pointer to a function for deallocating objects that will be added to this container
// while deleting this container, if NULL - there is no dealocating
container *container_new(int alloc_size, int elem_size, void (*dealloc_fn)(void *));

// create new container as a wrapper aroud @data
// when container being deleted @data woun't be deallocated
container *container_new_as_warapper(int size, int elem_size, void* data);

// delete existing container
// @c - pointer to a container that will be deallocated
void container_delete(container *c);

// add to @c an @element
void container_add_element(container *c, void *element);

// add to @c @count @elements that located continuously in the memory
void container_add_elements(container *c, void *elements, int count);

// remove from @c last element and return a pointer to it
// if nothing to remove - returns NULL;
void *container_remove_last_element(container *c);

// remove from @c @count elements and return a pointer to first of them from front
// if count more than number of elements in the container - returns NAN
void *container_remove_last_elements(container *c, int count);

// increase @c a @factor times
void container_increase_capacity(container *c, int factor);

// get pointer to element by @index position
void* container_get_element(container *c, int index);

// function for container deallocation if it is being putted to container as addres of pointer
void container_dealloc_as_pointer(void *c);

#endif // __CONTAINER_H__
