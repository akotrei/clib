#ifndef __LIST_H__
#define __LIST_H__

typedef struct list list; /* double linked list */

/*
    create list to storage any objects of size @elem_size.
    @dealloc_fn can be provided to delete objects in list when it is deleting.
    @dealloc_fn can be NULL - there no deleting in this case
*/
list *list_create(int elem_size, void (*dealloc_fn)(void *));

/*
    delete list @l
*/
void list_delete(list *l);

/*
    get number of elements in list @l
*/
int list_size(list *l);

/*
    add @elem to tail of list @l
*/
void list_add_tail(list *l, void *elem);

/*
    add @elem to head of list @l
*/
void list_add_head(list *l, void *elem);

/*
    get @elem from tail of list @l.
    returns NULL if nothing to get
*/
void *list_get_tail(list *l);

/*
    get @elem from head of list @l.
    returns NULL if nothing to get
*/
void *list_get_head(list *l);

/*
    get @elem from tail of list @l and delete it from @l.
    returns NULL if nothing to get
*/
void *list_remove_tail(list *l);

/*
    get @elem from head of list @l and delete it from @l.
    returns NULL if nothing to get
*/
void *list_remove_head(list *l);

#endif /* __LIST_H__ */