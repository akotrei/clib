#include "tree.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct knot
{
    void *data;
    knot *left;
    knot *right;
};

typedef struct tree
{
    int elem_size;
    int (*compare_fn)(void *, void *);
    void (*dealloc_fn)(void *);
    knot *knot_t;
};

static knot *knot_create(void *data)
{
    knot *knot_t = NULL;
    knot_t = (knot *)malloc(sizeof(knot));
    assert(knot_t != NULL && "memory for @knot_t hasn't been allocated");

    knot_t->data = data;
    knot_t->left = knot_t->right = NULL;
    return knot_t;
}

static void knot_print(knot *knot_t, void (*print_fn)(void *))
{
    if(knot_t == NULL)
        return;

    knot_print(knot_t->left, print_fn);
    print_fn(knot_t->data);
    knot_print(knot_t->right, print_fn);
}

static void knot_delete_all(knot **knot_t, void (*dealloc_fn)(void *))
{
    if(*knot_t == NULL)
        return; 

    knot_delete_all(&(*knot_t)->left, dealloc_fn);
    knot_delete_all(&(*knot_t)->right, dealloc_fn);
    dealloc_fn((*knot_t)->data);
    free(*knot_t);
}

static void knot_delete(knot *knot_t, void (*dealloc_fn)(void *data))
{
    dealloc_fn(knot_t->data);
    free(knot_t);
}

tree *tree_new(int elem_size, void (*dealloc_fn)(void *), int (*compare_fn)(void *, void *))
{
    tree *t = NULL;
    t = (tree *)malloc(sizeof(tree));
    assert(t != NULL && "memory for @tree hasn't been allocated");

    t->compare_fn = compare_fn;
    t->dealloc_fn = dealloc_fn;
    t->elem_size = elem_size;
    t->knot_t = NULL;
    return t;
}

void tree_add_elem(tree *t, void *elem)
{
    assert(t->compare_fn != NULL && "tree was created with @cpm_fn = NULL");

    knot **knot_t = &t->knot_t;

    while((*knot_t) != NULL)
    {
        if (t->compare_fn((*knot_t)->data, elem) <= 0)
            knot_t = &(*knot_t)->left;
        else
            knot_t = &(*knot_t)->right;
    }
    void *tmp_data = malloc(t->elem_size);
    memcpy(tmp_data, elem, t->elem_size);
    (*knot_t) = knot_create(tmp_data);
}

void tree_print(tree *t, void (*print_fn)(void *))
{
    knot_print(t->knot_t, print_fn);
}

void tree_delete(tree *t)
{
    knot_delete_all(&t->knot_t, t->dealloc_fn);
    free(t);
}

/*void tree_insert(tree *t, void *obj)
{
    tree *tmp = NULL, *t1 = t, *t2 = t;
    int result = 0;

    while (t != NULL)
    {
        result = t->compare_fn(t->data, obj);

        if (result == -1 || result == 0)
        {
            tmp = t;
            t1 = tmp;
            t = t->left;
            tmp->left = t;
            tmp = t;
        }

        if (result == 1 || result == 0)
        {
            tmp = t;
            t1 = tmp;
            t = t->right;
            tmp->right = t;
            tmp = t;
        }
    }

    if (result == -1 || result == 0)
        t1->left = tree_new(obj, t2->compare_fn);

    if (result == 1 || result == 0)
        t1->right = tree_new(obj, t2->compare_fn);
}

void tree_print(tree *t, void (*print_fn)(void *obj))
{
    if (t == NULL)
        return;

    tree_print(t->right, print_fn);
    print_fn(t->data);
    tree_print(t->left, print_fn);
}*/

/*=========================================================*/
/*typedef struct knot_t knot_t;

typedef struct tree
{
    int elem_size;
    int (*cmp_fn)(void *o1, void *o2);
    void (*dealloc_fn)(void *elem);
    knot_t *knot;
};

typedef struct knot_t
{
    void *data;
    knot_t *l;
    knot_t *r;
};

static knot_t *knot_create(void *data);
static void knot_delete(knot_t *knot, void (*dealloc_fn)(void *data));
static void knot_add_l(knot_t *knot, void *elem);
static void knot_add_r(knot_t *knot, void *elem);

static knot_t *knot_traverse(knot_t *knot, void *elem, int (*cmp_fn)(void *, void *));

tree *tree_create(int elem_size, int (*cmp_fn)(void *, void *), void (*dealloc_fn)(void *))
{
    tree *t = (tree *)malloc(sizeof(tree));
    t->elem_size = elem_size;
    t->cmp_fn = cmp_fn;
    t->dealloc_fn = dealloc_fn;
    t->knot = NULL;
    return t;
}

void tree_add_elem(tree *t, void *elem)
{
    knot_t *knot = t->knot;
    knot_t *knot_to_add;
    knot_to_add = knot_traverse(knot, elem, t->compare_fn);
}

knot_t *knot_traverse(knot_t *knot, void *elem, int (*cmp_fn)(void *, void *))
{
    knot_t* knot_traverse = knot;
    int cmp_result;
    while (knot_traverse)
    {
        cmp_result = cmp_fn(knot->data, elem);
        if (cmp_result <=0 )
        {
            knot_traverse = knot_traverse->l;
        }
        else
        {
            knot_traverse = knot_traverse->r;
        }
    }
    return knot_traverse;
}*/
