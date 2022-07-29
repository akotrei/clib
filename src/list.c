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

static void* node_delete(node_t* n, void (*deallocate)(void *, void *));

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

/* function @list_add_head to add an object to the list
 */
void list_add_head(list_t *list, void *obj)
{
    /**/
    if(list->head == NULL && list->tail == NULL)
    {
        /**/
        list->head = list->tail = (node_t *)node_create(list->copy_fn(obj), 
                                                        list->iallocator, 
                                                        NULL, 
                                                        NULL);
    }
    /**/
    else 
    {
        /**/
        node_t *tmp = list->head;

        /**/
        node_t *node = (node_t *)node_create(list->copy_fn(obj), 
                                             list->iallocator,
                                             tmp, 
                                             NULL);

        /**/
        list->head->prev = node;

        /**/
        list->head = node;
    }
}

/**/
void list_add_tail(list_t* list, void* obj)
{
    /**/
    if(list->tail == NULL)
    {
        /**/
        list->head = list->tail = (node_t *)node_create(list->copy_fn(obj), 
                                                        list->iallocator, 
                                                        NULL, 
                                                        NULL);
    }
    /**/
    else 
    {
        /**/
        node_t *tmp = list->tail;

        /**/
        node_t *node = (node_t *)node_create(list->copy_fn(obj), 
                                             list->iallocator, 
                                             NULL, 
                                             tmp);

        /**/
        list->tail->next = node;

        /**/
        list->tail = node;
    }
}

/**/
void* list_rmv_head(list_t* list)
{
    /**/
    void *data_of_deleting_node = list->head->data;
   
    /**/
    list->head = list->head->next;

    /**/
    list->iallocator->deallocate(NULL, list->head->prev);

    /**/
    list->head->prev = NULL;

    /**/
    return data_of_deleting_node;
}

/**/
void* list_rmv_tail(list_t* list)
{
    /**/
    void *data_of_deleting_node = list->tail->data;

    /**/
    list->tail = list->tail->prev;
    
    /**/
    list->iallocator->deallocate(NULL, list->tail->next);

    /**/
    list->tail->next = NULL;

    /**/
    return data_of_deleting_node;
}

/**/
void* list_rmv_shead(list_t* list, void* obj)
{
    /**/
    node_t *tmp = list->head;

    /**/
    while(tmp != NULL)
    {
        /**/
        if(list->compare_fn(tmp->data, obj) == 0)
        {
            /**/
            tmp->prev->next = tmp->next;

            /**/
            tmp->next->prev = tmp->prev;

            /**/
            void *data_of_deleting_node = tmp->data;

            /**/
            list->iallocator->deallocate(NULL, tmp);

            /**/
            return data_of_deleting_node;
        }

        /**/
        tmp = tmp->next;
    }

    /**/
    return NULL;
}

/**/
void* list_rmv_stail(list_t* list, void* obj)
{
    /**/
    node_t *tmp = list->tail;

    /**/
    while(tmp != NULL)
    {
        /**/
        if(list->compare_fn(tmp->data, obj) == 0)
        {
            /**/
            tmp->prev->next = tmp->next;

            /**/
            tmp->next->prev = tmp->prev;

            /**/
            void *data_of_deleting_node = tmp->data;

            /**/
            list->iallocator->deallocate(NULL, tmp);

            /**/
            return data_of_deleting_node;
        }

        /**/
        tmp = tmp->prev;
    }

    /**/
    return NULL;
}

/**/
void* list_fnd_shead(list_t* list, void* obj)
{
    /**/
    node_t *tmp = list->head;

    /**/
    while(tmp != NULL)
    {
        /**/
        if(list->compare_fn(tmp->data, obj) == 0)
        {
            /**/
            return tmp->data;
        }

        /**/
        tmp = tmp->next;
    }

    /**/
    return NULL;
}

/**/
void* list_fnd_stail(list_t* list, void* obj)
{
    /**/
    node_t *tmp = list->tail;

    /**/

    while(tmp != NULL)
    {
        /**/
        if(list->compare_fn(tmp->data, obj) == 0)
        {
            /**/
            return tmp->data;
        }

        /**/
        tmp = tmp->prev;
    }

    /**/
    return NULL;
}

/**/
void list_swap_objects(list_t* list, void *o1, void *o2)
{
    void **tmp_o1 = &list_fnd_shead(list, o1);
    void **tmp_o2 = &list_fnd_shead(list, o2);

    if(tmp_o1 != NULL && tmp_o2 != NULL)
    {
        void *tmp = *tmp_o1;
        *tmp_o1 = *tmp_o2;
        *tmp_o2 = *tmp;
    }
}

/**/
void list_clear(list_t *list)
{
    /**/
    node_t *node = list->head;

    /**/
    while(node != NULL)
    {
        /**/
        node_t *tmp = node;

        /**/
        node = node->next;

        /**/
        list->iallocator->deallocate(NULL, tmp);
    }

    /**/
    list->head = list->tail = NULL;
}

void list_prt_shead(list_t *list, void (*print_fn)(void *o))
{
    node_t **tmp = &list->head;
    while(*tmp != NULL)
    {
        printf("<---");
        print_fn((*tmp)->data);
        printf("--->");
        tmp = &(*tmp)->next;
    }
    printf("\n");
}

void list_prt_stail(list_t *list, void (*print_fn)(void *o))
{
    node_t **tmp = &list->tail;
    while(*tmp != NULL)
    {
        printf("<---");
        print_fn((*tmp)->data);
        printf("--->");
        tmp = &(*tmp)->prev;
    }
    printf("\n");
}

/**/
static node_t* node_create(void *data, iallocator_t *iallocator, 
                           node_t *next, node_t *prev)
{
    /**/
    node_t *node = (node_t *)iallocator->allocate(iallocator, sizeof(node_t));

    /**/
    node->data = data;
    node->next = next;
    node->prev = prev;

    /**/
    return node;
}

static void* node_delete(node_t* n, void (*deallocate)(void *, void *))
{
    void *data_of_deleting_node = n->data;
    deallocate(NULL, n);
    return data_of_deleting_node;
}
