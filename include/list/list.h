#ifndef __LIST_H__
#define __LIST_H__

#include "list_type_public.h"
#include "interface.h"

/* function @list_create list creation 
 *
 * @copy_fn       - pointer to a function that copies an object to a list node, 
 *                  if you pass a pointer to this function equal to zero when 
 *                  creating a list, then a function that returns a pointer 
 *                  to the object will be used, otherwise this object will be 
 *                  copied
 *
 * @dealloc_fn    - pointer to a function that deletes the list, if you pass 
 *                  a pointer to this function not equal to zero, then 
 *                  the list will be deleted using this function, otherwise 
 *                  a delete function will be passed that does nothing 
 *
 * @compare_fn    - pointer to a function that compares objects @o1, @o2 
 *                  and returns 0 if the objects are equal, otherwise returns -1
 *
 * @iallocator    - pointer to an allocator interface that will allocate 
 *                  and deallocate memory for the list and its nodes 
 *                  using functions @malloc and @free
 */
list_t* list_create(void* (*copy_fn)(void* obj),
                    void (*dealloc_fn)(void* obj),
                    int (*compare_fn)(void* obj1, void* obj2),
                    iallocator_t* iallocator);

/* function @list_delete removes all nodes in the list
 *
 * @list    - pointer to list
 */
void list_delete(list_t* list);

/* function @list_add_head of adding an object to the beginning of the list 
 * (the object being added will be at the head of the list)
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void list_add_head(list_t* list, void* obj);

/* function @list_add_tail of adding an object to the end of the list 
 * (the object being added will be at the tail of the list)
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void list_add_tail(list_t* list, void* obj);

/* function @list_rmv_head to remove the head of the list
 *
 * @list    - pointer to list
 */
void* list_rmv_head(list_t* list);

/* function @list_rmv_tail to remove the tail of the list
 *
 * @list    - pointer to list
 */
void* list_rmv_tail(list_t* list);

/* function @list_rmv_shead to delete a node in the list starting from its head
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void* list_rmv_shead(list_t* list, void* obj);

/* function @list_rmv_stail to delete a node in the list starting from its tail
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void* list_rmv_stail(list_t* list, void* obj);

/* function @list_fnd_shead to search for an object in 
 * the list starting from its head
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void* list_fnd_shead(list_t* list, void* obj);

/* function @list_fnd_stail to search for an object in 
 * the list starting from its tail
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void* list_fnd_stail(list_t* list, void* obj);

/* function @list_swap_objects object exchange @o1, @o2
 *
 * @list    - pointer to list
 *
 * @o1      - pointer to object
 *
 * @o2      - pointer to obejct 
 */
void list_swap_objects(list_t *list, void *o1, void *o2);

/* function to remove all nodes from the list, i.e. the list becomes empty
 *
 * @list    - pointer to list
 */
void list_clear(list_t *list);



void* list_get_head(list_t* list);

void* list_get_tail(list_t* list);

void* list_get_next(list_t* list);

void* list_get_prev(list_t* list);
void list_prt_shead(list_t *list, void (*print_fn)(void *o));
void list_prt_stail(list_t *list, void (*print_fn)(void *o));

#endif /* __LIST_H__ */
