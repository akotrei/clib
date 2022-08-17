#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "array_type_public.h"
#include "interface.h"

/* @array_create function returns a pointer to the created array
 *
 * @factor     - the value by which the size of the array memory will be increased
 *               note: the variable must be > 1 otherwise the memory for
 *               the array will not increase
 *               note: if this parameter is equal 0 then @factor will be equal 2
 *
 * @alloc_size - memory allocated for working with an array
 *               note: variable must be > 0 to work with array
 *
 * @elem_size  - the size of the element in the array
 *
 * @copy_fn    - function of copying an element to an array if the pointer to it
 *               is not equal NULL, otherwise the function will return
 *               a pointer to the object
 *
 * @dealloc_fn - function to delete an object in an array if the pointer to
 *               it is not equal NULL, otherwise the function does nothing
 *
 * @compare_fn - object comparison function if the pointer to it is not equal
 *               to zero, if the objects are equal then the function returns 0,
 *               otherwise -1
 *               note: the function is needed to get index of element
 *               in the array
 *
 * @iallocator - pointer to an allocator interface that implements copy,
 *               reallocate, and allocate deallocation functions
 *               note: if the interface pointer is null then it
 *               is created when the array is created
 *
 */
array_t*
array_create(int alloc_size,
             int elem_size,
             void* (*copy_fn)(void *o),
             void (*dealloc_fn)(void *o),
             int (*compare_fn)(const void *o1, const void *o2),
             int factor,
             iallocator_t *iallocator);

/* @array_delete array delete function
 *
 * @a - pointer to array
 */
void
array_delete(array_t *a);

/* @array_push_back function to insert an object at the end of an array
 *
 * @a   - pointer to array
 *
 * @obj - pointer to object
 */
void
array_push_back(array_t *a,
                void *obj);

/* @array_insert function to add elements to an array starting at a specified index
 *
 * @a     - pointer to array
 *
 * @obj   - pointer to object
 *
 * @index - index of element in array
 *
 * @count - number of items to add
 */
void
array_insert(array_t *a,
             void *obj,
             int index,
             int count);

/* @array_fnd_element function to search for an element by index and returns 
 * a pointer to the found element
 *
 * @a     - pointer to array
 *
 * @obj   - pointer to object
 *
 * @index - index of element in array
 */
void*
array_get_data(array_t *a,
               int index);

/* @array_get_index function returns the index of the object in the array
 *
 * @a     - pointer to array
 *
 * @obj   - pointer to object
 *
 * @index - index of element in array
 */
int
array_get_index(array_t *a,
                void *obj);

/* @array_rmv_element function to remove an element from an array by index 
 *
 * @a     - pointer to array
 *
 * @obj   - pointer to object
 *
 * @index - index of element in array
 */
void
array_rmv_element(array_t *a,
                  int index);

/* @array_factor function changes the memory allocation rate for an array
 *
 * @a     - pointer to array
 *
 * @obj   - pointer to object
 *
 * @index - index of element in array
 */
void 
array_factor(array_t *a,
             int factor);

/* @array_size function returns the size of the array
 *
 * @a - pointer to array
 */
int 
array_size(array_t *a);

/* @array_print function to print an array to the console
 *
 * @a     - pointer to array
 *
 * @obj   - pointer to object
 *
 * @index - index of element in array
 */
void
array_print(array_t *a,
            void (*print_fn)(void *o));

#endif /* __ARRAY_H__ */
