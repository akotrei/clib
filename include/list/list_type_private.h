#ifndef __LIST_TYPE_PRIVATE_H__
#define __LIST_TYPE_PRIVATE_H__

/*
 * List structure definiton
 */
typedef struct _list_t
{
    /* Number of nodes in the list*/
    int list_size;

    /* Pointer to an allocator interface that allocates and frees memory*/
    iallocator_t *iallocator;

    /* Pointer to a function that copies the object*/
    void* (*copy_fn)(void *obj);

    /* Pointer to a function that removes the object*/
    void (*dealloc_fn)(void *obj);

    /*
     * Pointer to a function that compares objects and returns 0 if they
     * are equal
     */
    int (*compare_fn)(void *obj1, void *obj2);

    /* Pointer to the head of the list*/
    node_t *head;

    /* Pointer to the current node of the list*/
    node_t *curr;

    /* Pointer to the tail of the list*/
    node_t *tail;
} _list_t;

/*
 * List node structure definition
 */
typedef struct _node_t
{
    /* Pointer to data in node*/
    void *data;

    /* Pointer to next node*/
    node_t *next;

    /* Pointer to previous node*/
    node_t *prev;

} _node_t;

#endif /* __LIST_TYPE_PRIVATE_H__ */
