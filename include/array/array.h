#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "array_type_public.h"
#include "interface.h"

/* @array_create function returns a pointer to the created array
 * 
 * @factor     - the value by which the size of the array memory will be increased
 *               note: the variable must be > 1 otherwise the memory for 
 *               the array will not increase
 *
 * @alloc_size - memory allocated for working with an array
 * 				 note: variable must be > 0 to work with array
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
 *               note: the function is needed to remove and find elements 
 *               in the array
 * 
 * @iallocator - pointer to an allocator interface that implements copy, 
 *               reallocate, and allocate deallocation functions
 *               note: if the interface pointer is null then it 
 *               is created when the array is created
 *
 */
array_t *array_create(int factor, int alloc_size, int elem_size, void* (*copy_fn)(void *o), void (*dealloc_fn)(void *o), int (*compare_fn)(void *o1, void *o2), iallocator_t *iallocator);

/* @array_delete array delete function 
 *
 * @a - pointer to array
 */
void array_delete(array_t *a);

/* @array_push_back function to insert an object at the end of an array
 *
 * @a   - pointer to array
 *
 * @obj - pointer to object
 */
void array_push_back(array_t *a, void *obj);

/* @array_push_front function adds an object to the beginning of the array
 *
 * @a   - pointer to array
 *
 * @obj - pointer to object
 */
void array_push_front(array_t *a, void *obj);

/* @array_insert_obj_by_index function to insert an object into an array by index
 * note: if index < 0 then the object will be inserted at the beginning 
 * of the array, if more than the number of array elements, then it will 
 * be inserted at the end of the array
 *
 * @a     - pointer to array
 * 
 * @obj   - pointer to object
 *
 * @index - index of element in array
 */
void array_insert_obj_by_index(array_t *a, void *obj, int index);

/* @array_fnd_obj function to search for an object in an array
 * note: if the object is not found then NULL will be returned
 *
 * @a     - pointer to array
 *
 * @obj   - pointer to object
 */
void* array_fnd_obj(array_t *a, void *obj);

/* @array_fnd_obj_by_index function to search for an object in an array by index
 * note: if the index is out of bounds of the array, then the object 
 * that lies at the beginning of the array or at the end, 
 * respectively, is returned
 *
 * @a     - pointer to array
 * 
 * @obj   - pointer to object
 *
 * @index - index of element in array
 */
void* array_fnd_obj_by_index(array_t *a, int index);

/* @array_rmv_obj function to remove an object from an array
 * note: if the object to be deleted is not found, then NULL will be returned 
 *
 * @a     - pointer to array
 *
 * @obj   - pointer to object
 */
void* array_rmv_obj(array_t *a, void *obj);

/* @array_rmv_obj_by_index function to remove an object from an array by index
 * note: if the index is out of bounds of the array, then the object 
 * that lies at the beginning of the array or at the end, 
 * respectively, is returned
 *
 * @a     - pointer to array
 *
 * @index - index of element in array
 */
void* array_rmv_obj_by_index(array_t *a, int index);

/* @array_print function to print an array to the console
 *
 * @a        - pointer to array
 *
 * @print_fn - pointer to the function to print the object to the console
 */
void array_print(array_t *a, void (*print_fn)(void *o));

#endif /* __ARRAY_H__ */
