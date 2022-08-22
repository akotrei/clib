#include "list.h"
#include "allocator_std.h"
#include "list_type_private.h"

#include <stdlib.h>
#include <stdio.h>

/* function @dealloc_fake doesn't delete anything if NULL is passed when 
 * creating the list instead of the object deletion function
 *
 * @o    - pointer to object
 */
inline static void dealloc_fake(void *o) {};

/* function @copy_fake that returns a pointer to an object if zero was passed 
 * when creating the list instead of the object copy function
 *
 * @o    - pointer to obejct
 */
inline static void* copy_fake(void *o) { return o; };

/* function @node_create creates a node in the list and returns a pointer 
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

/* function @node_fnd finds an object in the list and 
 * returns the node in which the found object is located
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
static node_t* node_fnd(list_t *list, void *obj);

/* function @list_create that creates a list and returns 
 * a pointer to the created list
 */
list_t* list_create(void* (*copy_fn)(void *obj),
                    void (*dealloc_fn)(void *obj),
                    int (*compare_fn)(void *obj1, void *obj2),
                    iallocator_t* iallocator)
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
    
    /*list creation*/
    list_t *l = (list_t *)_iallocator->allocate(_iallocator->self, 
                                                sizeof(list_t));

    /*list structure field initialization*/
    l->list_size = 0;
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

    /*returning a pointer to the list being created*/
    return l;
}

/* function @list_delete list delete
 *
 * @list    - pointer to list
 */
void list_delete(list_t* list)
{
    /*pointer to the head of the list*/
    node_t *node = list->head;

    /*pointer to delete function*/
    void (*deallocate)(void *self, void *address) = list->iallocator->deallocate;

    /*loop to traverse the list*/
    while(node != NULL)
    {
        /*deleting data from a node*/
        list->dealloc_fn(node->data);

        /*temporary pointer to the node to be removed*/
        node_t *tmp = node;

        /*move to the next list node to remove it*/
        node = node->next;

        /*deleting a list node*/
        deallocate(NULL, tmp);
    }

    /* if the allocator was created when the list was created, 
     * then it will be deleted when the list is deleted*/
    if(list->iallocator_owner == 1)
    {
        /*allocator removal*/
        allocator_std_delete(list->iallocator->self);
    }

    /*deleting a list*/
    deallocate(NULL, list);
}

/* function @list_add_head to add an object to the top of the list
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void list_add_head(list_t *list, void *obj)
{
    /*if the list is empty, then we do this check*/
    if(list->head == NULL && list->tail == NULL)
    {
        /*creating a node in a list that will be both 
         * head and tail at the same time*/
        list->head = list->tail = (node_t *)node_create(list->copy_fn(obj), 
                                                        list->iallocator, 
                                                        NULL, 
                                                        NULL);

        /*increasing the size of the list when adding a node*/ 
        list->list_size++;
    }
    /*if nodes already exist in the list*/
    else 
    {
        /*another pointer to the head of the list*/
        node_t *tmp = list->head;

        /*creating a new node*/
        node_t *node = (node_t *)node_create(list->copy_fn(obj), 
                                             list->iallocator,
                                             tmp, 
                                             NULL);

        /*at the head of the list, the pointer points to the newly created node*/
        list->head->prev = node;

        /*now the head of the list is the newly created node*/
        list->head = node;

        /*increasing the size of the list when adding a node*/ 
        list->list_size++;
    }
}

/* function @list_add_tail to add an object to the end of the list
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void list_add_tail(list_t* list, void* obj)
{
    /*if the list is empty, then we do this check*/
    if(list->tail == NULL && list->head == NULL)
    {
        /*creating a node in a list that will be both 
         * head and tail at the same time*/
        list->head = list->tail = (node_t *)node_create(list->copy_fn(obj), 
                                                        list->iallocator, 
                                                        NULL, 
                                                        NULL);

        /*increasing the size of the list when adding a node*/ 
        list->list_size++;
    }
    /*if nodes already exist in the list*/
    else 
    {
        /*another pointer to the tail of the list*/
        node_t *tmp = list->tail;

        /*creating a new node*/
        node_t *node = (node_t *)node_create(list->copy_fn(obj), 
                                             list->iallocator, 
                                             NULL, 
                                             tmp);

        /*the pointer from the tail of the list points to the created node*/
        list->tail->next = node;

        /*now the tail of the list is the created node*/
        list->tail = node;

        /*increasing the size of the list when adding a node*/ 
        list->list_size++;
    }
}

/* function @list_rmv_head to remove the head from the list 
 *
 * list    - pointer to list
 */
void* list_rmv_head(list_t* list)
{
    /*pointer to the data in the node to be removed*/
    void *data_of_deleting_node = list->head->data;

    /*if list just have a head of the list*/
    if(list->head->next == NULL && list->head->prev == NULL)
    {
        /*removing the head of the list*/
        list->iallocator->deallocate(NULL, list->head);

        /*tail of the list is NULL*/
        list->head = NULL;

        /*decreasing the size of the list when deleting a node*/ 
        list->list_size--;

        /*returning data from the node being deleted*/
        return data_of_deleting_node;
    }

    /*now the next node in the list after the head becomes the head*/
    list->head = list->head->next;

    /*removing the head of the list*/
    list->iallocator->deallocate(NULL, list->head->prev);

    /*pointer in head to previous node is now NULL*/
    list->head->prev = NULL;

    /*decreasing the size of the list when deleting a node*/ 
    list->list_size--;

    /*returning data from the node being deleted*/
    return data_of_deleting_node;
}

/* function @list_rmv_head to remove the head from the list 
 *
 * list    - pointer to list
 */
void* list_rmv_tail(list_t* list)
{
    /*pointer to the data in the node to be removed*/
    void *data_of_deleting_node = list->tail->data;

    /*if list just have a tail of the list*/
    if(list->tail->next == NULL && list->tail->prev == NULL)
    {
        /*removing the tail of the list*/
        list->iallocator->deallocate(NULL, list->tail);

        /*tail of the list is NULL*/
        list->tail = NULL;

        /*decreasing the size of the list when deleting a node*/ 
        list->list_size--;

        /*returning data from the node being deleted*/
        return data_of_deleting_node;
    }


    /*now the tail pointer points to the penultimate node of the list*/
    list->tail = list->tail->prev;
    
    /*removing the tail of the list*/
    list->iallocator->deallocate(NULL, list->tail->next);

    /*pointer in tail to next node is now NULL*/
    list->tail->next = NULL;

    /*decreasing the size of the list when deleting a node*/ 
    list->list_size--;

    /*returning data from the node being deleted*/
    return data_of_deleting_node;
}

/* function @list_rmv_shead removes a node from the list starting from its head
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void* list_rmv_shead(list_t* list, void* obj)
{
    /*list traversal pointer*/
    node_t *tmp = list->head;

    /*loop to traverse the list to remove the desired node*/
    while(tmp != NULL)
    {
        /*if the node in which the data for deleting lies is equal 
         * to the data passed to the function, then delete the node*/
        if(list->compare_fn(tmp->data, obj) == 0)
        {
            /*when there is only one element in the list*/
            if(tmp->prev == NULL && tmp->next == NULL)
            {
                /*returning data from the node being deleted*/
                return list_rmv_head(list);
            }

            /*when found node is head*/
            if(tmp->prev == NULL && tmp->next != NULL)
            {
                /*returning data from the node being deleted*/
                return list_rmv_head(list);
            }

            /*when found node is tail*/
            if(tmp->prev != NULL && tmp->next == NULL)
            {
                /*returning data from the node being deleted*/
                return list_rmv_tail(list);
            }

            /*when the node is between the head and tail of the list*/
            if(tmp->next != NULL && tmp->prev != NULL)
            {
                /*the pointer from the previous node points to the next 
                 * node relative to the node being removed*/
                tmp->prev->next = tmp->next;

                /*the pointer from the next node points to the previous 
                 * node relative to the node being removed*/
                tmp->next->prev = tmp->prev;

                /*pointer to data from node to be removed*/
                void *data_of_deleting_node = tmp->data;

                /*deleting a node*/
                list->iallocator->deallocate(NULL, tmp);

                /*decreasing the size of the list when deleting a node*/ 
                list->list_size--;

                /*returning data from the node being deleted*/
                return data_of_deleting_node;
            }
        }

        /*move to the next node in the list*/
        tmp = tmp->next;
    }

    /*if the required node for deletion was not found, then return NULL*/
    return NULL;
}

/* function @list_rmv_shead removes a node from the list starting from its tail
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void* list_rmv_stail(list_t* list, void* obj)
{
    /*list traversal pointer*/
    node_t *tmp = list->tail;

    /*loop to traverse the list to remove the desired node*/
    while(tmp != NULL)
    {
        /*if the node in which the data for deleting lies is equal 
         * to the data passed to the function, then delete the node*/
        if(list->compare_fn(tmp->data, obj) == 0)
        {
            /*when there is only one element in the list*/
            if(tmp->prev == NULL && tmp->next == NULL)
            {
                /*returning data from the node being deleted*/
                return list_rmv_head(list);
            }

            /*when found node is head*/
            if(tmp->prev == NULL && tmp->next != NULL)
            {
                /*returning data from the node being deleted*/
                return list_rmv_head(list);
            }

            /*when found node is tail*/
            if(tmp->prev != NULL && tmp->next == NULL)
            {
                /*returning data from the node being deleted*/
                return list_rmv_tail(list);
            }

            /*when the node is between the head and tail of the list*/
            if(tmp->prev != NULL && tmp->next != NULL)
            {
                /*the pointer from the previous node points to the next 
                 * node relative to the node being removed*/
                tmp->prev->next = tmp->next;

                /*the pointer from the next node points to the previous 
                 * node relative to the node being removed*/
                tmp->next->prev = tmp->prev;

                /*pointer to data from node to be removed*/
                void *data_of_deleting_node = tmp->data;

                /*deleting a node*/
                list->iallocator->deallocate(NULL, tmp);

                /*decreasing the size of the list when deleting a node*/ 
                list->list_size--;

                /*returning data from the node being deleted*/
                return data_of_deleting_node;
            }
        }

        /*move to the previous node in the list*/
        tmp = tmp->prev;
    }

    /*if the required node for deletion was not found, then return NULL*/
    return NULL;
}

/* function @list_fnd_shead finds an object in the list starting from its head
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void* list_fnd_shead(list_t* list, void* obj)
{
    /*list traversal pointer*/
    node_t *tmp = list->head;

    /*loop to traverse the list to remove the desired node*/
    while(tmp != NULL)
    {
        /*condition for finding the desired list node*/
        if(list->compare_fn(tmp->data, obj) == 0)
        {
            /*return data from found node*/
            return tmp->data;
        }

        /*move to the next node in the list*/
        tmp = tmp->next;
    }

    /*return NULL if the required object was not found*/
    return NULL;
}

/* function @list_fnd_shead finds an object in the list starting from its tail
 *
 * @list    - pointer to list
 *
 * @obj     - pointer to object
 */
void* list_fnd_stail(list_t* list, void* obj)
{
    /*list traversal pointer*/
    node_t *tmp = list->tail;

    /*loop to traverse the list to remove the desired node*/
    while(tmp != NULL)
    {
        /*condition for finding the desired list node*/
        if(list->compare_fn(tmp->data, obj) == 0)
        {
            /*return data from found node*/
            return tmp->data;
        }

        /*move to the previous node in the list*/
        tmp = tmp->prev;
    }

    /*return NULL if the required object was not found*/
    return NULL;
}

/* function @list_swap_objects swaps the data in the nodes of the list
 *
 * @list    - pointer to list
 *
 * @o1      - pointer to object
 *
 * @o2      - pointer to object
 */
void list_swap_objects(list_t* list, void *o1, void *o2)
{
    /*found nodes that contain data for exchange*/
    node_t *node_1 = node_fnd(list, o1);
    node_t *node_2 = node_fnd(list, o2);

    /*if the data was found, then swap them*/
    if(node_1 != NULL && node_2 != NULL)
    {
        void *tmp = node_1->data;
        node_1->data = node_2->data;
        node_2->data = tmp;
    }
    /*otherwise exit the function*/
    else 
    {
        /*exit*/
        return;
    }
}

/* function @list_clear removes all nodes in the list making it empty
 *
 * @list    - pointer to list
 */
void list_clear(list_t *list)
{
    /*list traversal pointer*/
    node_t *node = list->head;

    /*loop to traverse the list to remove the desired node*/
    while(node != NULL)
    {
        /*another pointer to the current node*/
        node_t *tmp = node;

        /*deleting a data from node*/
        list->dealloc_fn(tmp->data);

        /*move to the next node in the list*/
        node = node->next;

        /*deleting a node*/
        list->iallocator->deallocate(NULL, tmp);
    }

    /*after removing all nodes, their number is 0*/
    list->list_size = 0;

    /*pointers to the head and tail after deleting all nodes are made NULL*/
    list->head = list->tail = NULL;
}

/* function @list_size returns the size of the list
 *
 * @list    - pointer to list
 */
int list_size(list_t *list) 
{ 
    /*returning size of the list*/
    return list->list_size; 
}

node_t* list_get_head(list_t* list)
{
    /*returning the head of the list*/
    return list->head;
}

node_t* list_get_tail(list_t* list)
{
    /*returning the tail of the list*/
    return list->tail;
}

node_t* list_get_next(node_t* node)
{
    /*returning the next node of the current node*/
    return node->next;
}

node_t* list_get_prev(node_t* node)
{
    /*returning the previous node of the current node*/
    return node->prev;
}

/* function @list_prt_shead prints the list starting from its head
 *
 * @list        - pointer to list
 *
 * @print_fn    - pointer to the object's print function
 */
void list_prt_shead(list_t *list, void (*print_fn)(void *o))
{
    /*pointer to pointer to head of list*/
    node_t **tmp = &list->head;

    /*loop to traverse the list*/
    while(*tmp != NULL)
    {
        /*data printing*/
        printf("<---");
        print_fn((*tmp)->data);
        printf("--->");

        /*jump to next node*/
        tmp = &(*tmp)->next;
    }
    printf("\n");
}

void list_prt_stail(list_t *list, void (*print_fn)(void *o))
{
    /*pointer to pointer to head of list*/
    node_t **tmp = &list->tail;

    /*loop to traverse the list*/
    while(*tmp != NULL)
    {
        /*data printing*/
        printf("<---");
        print_fn((*tmp)->data);
        printf("--->");

        /*jump to next node*/
        tmp = &(*tmp)->prev;
    }
    printf("\n");
}

static node_t* node_create(void *data, iallocator_t *iallocator, 
                           node_t *next, node_t *prev)
{
    /*creating a new node*/
    node_t *node = (node_t *)iallocator->allocate(iallocator, sizeof(node_t));

    /*initializing the fields of the list node structure*/
    node->data = data;
    node->next = next;
    node->prev = prev;

    /*return the created node*/
    return node;
}

static node_t* node_fnd(list_t *list, void *obj)
{
    /*list traversal pointer*/
    node_t *tmp = list->head;

    /*loop to traverse the list to remove the desired node*/
    while(tmp != NULL)
    {
        /*condition for finding the desired list node*/
        if(list->compare_fn(tmp->data, obj) == 0)
        {
            /*return found node*/
            return tmp;
        }

        /*move to the next node in the list*/
        tmp = tmp->next;
    }

    /*return NULL if the required object was not found*/
    return NULL;
}
