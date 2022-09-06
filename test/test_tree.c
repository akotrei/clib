#include "tree.h"
#include "tree_type_private.h"
#include "allocator_std.h"
#include "tree_iterator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* declaration type of strcture student for testing the tree*/
typedef struct student
{
    int id;
    char *name;
    char *first_name;
    char *last_name;
    double mark;
    int age;
    char sex;
} student;

/* structure of point for testing adding and removing functions of the tree*/
typedef struct point
{
    double x;
    double y;
    double z;
} point;

/* structure of tree with id to be able to add tree to tree
 * note: the identifier is needed for the comparison function in the tree*/
typedef struct tree_struct
{
    int id;
    tree_t *tree;
} tree_struct;

int cmp_fn(void *o1, void *o2);
int any_cmp_fn(void *o1, void *o2);
void dealloc_fn(void *o);
void* copy_fn(void *o);
void* copy_point_fn(void *o);
void* pointer_copy_fn(void *o);
void pointer_dealloc(void *o);
void print_fn(void *o);
void print_point(void *o);

int test_1();

int main(int argc, char **args)
{
    printf("======================================================= TEST 1 =======================================================\n");
    printf("SCRIPT DESCRIPTION\n");
    printf("Creating a tree without using the functions of copying and deleting objects,\n");
    printf("only the function of comparing objects, since this is the main function for working with a tree,\n");
    printf("so that you can fill the tree with objects. Testing adding an object to a tree, deleting a tree.\n\n");
    test_1() == 0 ? printf("TEST 1 HAS BEEN PASSED\n") : printf("TEST 1 HASN'T BEEN PASSED\n");
    printf("======================================================================================================================\n\n");

    return 0;
}

int cmp_fn(void *o1, void *o2)
{
    student *o_1 = *(student **)o1;
    student *o_2 = *(student **)o2;
    if(o_1->id < o_2->id)
        return 1;
    if(o_1->id > o_2->id)
        return -1;
    return 0;
}

int any_cmp_fn(void *o1, void *o2)
{
    point *o_1 = (point *)o1;
    point *o_2 = (point *)o2;
    if(o_1->x < o_2->x)
        return 1;
    if(o_1->x > o_2->x)
        return -1;
    return 0;
}

void dealloc_fn(void *o)
{
    free(*(student **)o);
}

void* copy_fn(void *o)
{
    void *result = malloc(sizeof(student));
    memcpy(result, o, sizeof(student));
    return result;
}

void* copy_point_fn(void *o)
{
    void *copied_point = malloc(sizeof(point));
    memcpy(copied_point, o, sizeof(point));
    return copied_point;
}

void* pointer_copy_fn(void *o)
{
    void *copy_result = malloc(sizeof(student *));
    memcpy(copy_result, o, sizeof(student *));
    return copy_result;
}

void pointer_dealloc(void *o)
{
    free((student **)o);
}

void print_fn(void *o)
{
    student *s = *(student **)o;
    printf("id: %d\nfirst_name: %s\nname: %s\nlast_name: %s\nmark: %.2lf\nage: %d\nsex: %c\n", s->id, s->first_name, s->name, s->last_name, s->mark, s->age, s->sex);
}

void print_point(void *o)
{
    point *p = (point *)o;
    printf("[%.3lf, %.3lf, %.3lf]", p->x, p->y, p->z);
}

int test_1()
{
    char mem[sizeof(iallocator_t)];
    iallocator_t *iallocator = allocator_std_new(mem);

    char iter_mem[sizeof(itree_iterator_t)];
    itree_iterator_t *itree_iterator = tree_iterator_create(iter_mem);

    tree_t *t = NULL;
    t = tree_create(iallocator->allocate(NULL, tree_sizeof()),
                    any_cmp_fn,
                    NULL,
                    NULL,
                    iallocator);

    point *o = malloc(sizeof(point));
    o->x = 5;
    o->y = 3;
    o->z = 2;

    point *o1 = malloc(sizeof(point));
    o1->x = 8;
    o1->y = 5;
    o1->z = 4;

    point *o2 = malloc(sizeof(point));
    o2->x = 12;
    o2->y = 7;
    o2->z = 1;

    point *o3 = malloc(sizeof(point));
    o3->x = 13;
    o3->y = 7;
    o3->z = 1;

    tree_add_object(t, o);
    tree_add_object(t, o1);
    tree_add_object(t, o2);
    tree_add_object(t, o3);

    printf("address of @o: %p, address of data from the root knot: %p\n", o, t->root->data);
    printf("address of left knot: %p, address of right knot: %p, address of parent knot: %p\n", t->root->left, t->root->right, t->root->parent);
    print_point(t->root->data);printf("\n");
    printf("address of @o1 in the tree: %p, address of data from the root knot: %p\n", t->root->right->parent, t->root);
    printf("address of @o2 in the tree: %p, address of @o2: %p\n", t->root->right->right->data, o2);
    print_point(t->root->right->right->data);printf("\n");

    //tree_balance_DSW(t);

    print_point(itree_iterator->get_data(itree_iterator->curr(t)));
    itree_iterator->right(t);
    print_point(itree_iterator->get_data(itree_iterator->curr(t)));
    itree_iterator->right(t);
    print_point(itree_iterator->get_data(itree_iterator->curr(t)));
    itree_iterator->parent(t);
    print_point(itree_iterator->get_data(itree_iterator->curr(t)));
    itree_iterator->reset(t);
    print_point(itree_iterator->get_data(itree_iterator->curr(t)));

    print_point(tree_rmv_object(t, o1));
    tree_delete(t);

    free(o);
    free(o1);
    free(o2);
    free(o3);

    return 0;
}
