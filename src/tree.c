#include "tree.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


tree *tree_new(void *obj, int (*compare_fn)(void *obj1, void *obj2))
{
    tree *t = (tree *)malloc(sizeof(tree));
    t->compare_fn = compare_fn;
    t->data = obj;
    t->left = t->right = NULL;
    return t;
}

void tree_insert(tree *t, void *obj)
{
    tree *tmp = NULL, *t1 = t, *t2 = t;
    int result = 0;

    while(t != NULL)
    {
        result = t->compare_fn(t->data, obj);

        if(result == -1 || result || == 0)
        {
            tmp = t;
            t1 = tmp;
            t = t->left;
            tmp->left = t;
            tmp = t;
        }

        if(result == 1 || result == 0)
        {
            tmp = t;
            t1 = tmp;
            t = t->right;
            tmp->right = t;
            tmp = t;
        }
    }

    if(result == -1 || result || == 0)
    {
        t1->left = (tree *)malloc(sizeof(tree));
        t1->left->compare_fn = t2->compare_fn;
        t1->left->data = obj;
        t1->left->left = t1->left->right = NULL;
    }

    if(result == 1 || result == 0)
    {
        t1->right = (tree *)malloc(sizeof(tree));
        t1->right->compare_fn = t2->compare_fn;
        t1->right->data = obj;
        t1->right->left = t1->right->right = NULL;
    }
}

void tree_print(tree *t, void (*print_fn)(void *obj))
{
    if(t == NULL)
        return;

    tree_print(t->right, print_fn);
    print_fn(t->data);
    tree_print(t->left, print_fn);
}
