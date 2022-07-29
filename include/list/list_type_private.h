#ifndef __LIST_TYPE_PRIVATE_H__
#define __LIST_TYPE_PRIVATE_H__

/*
 * list structure definiton
 */
typedef struct _list_t
{
    /* pointer to an allocator interface that allocates and frees memory*/
    iallocator_t *iallocator;

    /* if user passes iallocator equals NULL than tree creates internal 
     * allocator_std and is owner of this allocator and responsible 
     * for deleting this allocator @iallocator_owner equals 1 when 
     * tree is owner else 0*/
    int iallocator_owner;

    /* pointer to a function that copies the object*/
    void* (*copy_fn)(void *obj);

    /* pointer to a function that removes the object*/
    void (*dealloc_fn)(void *obj);

    /* pointer to a function that compares objects and returns 0 if they 
     * are equal*/
    int (*compare_fn)(void *obj1, void *obj2);

    /* pointer to the head of the list*/
    node_t *head;

    /* pointer to the tail of the list*/
    node_t *tail;
} _list_t;

/*
 * list node structure definition
 */
typedef struct _node_t
{
    /* pointer to data in node*/
    void *data;

    /* pointer to next node*/
    node_t *next;

    /* pointer to previous node*/
    node_t *prev;
} _node_t;

#endif /* __LIST_TYPE_PRIVATE_H__ */
