#include "list.h"

typedef struct node node;

typedef struct list
{
    int elem_size;
    int size;
    node* head;
    node* tail;
    void (*dealloc_fn)(void*);
};

typedef struct node
{
    void* elem;
    node* next;
    node* prev;
};

/*
    create new node and write to it @elem
*/
static node* node_create(int elem_size, void* elem);

/*
    delete node @n and invoke @dealloc_fn on nodes elem to delete it.
    @dealloc_fn can be NULL - there no deleting in this case
*/
static void node_delete(node* n, void (*dealloc_fn)(void*));
