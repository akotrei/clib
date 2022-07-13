#include "container.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct object
{
    int x;
    int y;
} object;

void test_container_1();
void test_container_2();
void test_container_3();
void free_obj(void *o);

int test_tree_cmp_fn(void *obj1, void *obj2);
void test_tree_1();
void test_tree_delete(void *);
void print(void *o);

int main(int argc, char **args)
{
//    test_container_1();
//    test_container_2();
//    test_container_3();
    test_tree_1();
    return 0;
}

void print(void *o)
{
    int *a = (int *)o;
    printf("[%d]", *a);
}

int test_tree_cmp_fn(void *obj1, void *obj2)
{
    int *tmp_obj1 = (int *)obj1;
    int *tmp_obj2 = (int *)obj2;
    if(*tmp_obj1 < *tmp_obj2)
        return 1;
    if(*tmp_obj1 > *tmp_obj2)
        return -1;
    return 0;
}

void test_tree_delete(void *o)
{
        int *a = (int*)o;
        free(a);
}

void test_tree_1()
{
    int a;// = (int *)malloc(sizeof(int));
    int b;// = (int *)malloc(sizeof(int));
    int d;// = (int *)malloc(sizeof(int));
    int f;// = (int *)malloc(sizeof(int));
    int c;// = (int *)malloc(sizeof(int));
    int e,g,h,j,i;
    a = 1;
    b = 2;
    c = 3;
    d = 4;
    e = 5;
    f = 6;
    g = 7;
    h = 8;
    i = 9;
    j = 10;
    tree_t *t = tree_create(test_tree_cmp_fn, NULL, NULL/*test_tree_delete*/, NULL);

    tree_add_object(t, &f);
    tree_add_object(t, &d);
    tree_add_object(t, &e);
    tree_add_object(t, &i);
    tree_add_object(t, &j);
    tree_add_object(t, &h);
    tree_add_object(t, &g);
    tree_add_object(t, &b);
    tree_add_object(t, &a);
    tree_add_object(t, &c);

    tree_print(t, print);
    
    void *p = tree_fnd_object(t, &d);
    printf("\n\n%p\n", p);
//    tree_fnd_object(t, f);

    tree_rmv_object(t, &f);
    tree_rmv_object(t, &i);

    tree_test_addresses(t, print);

//    tree_rmv_object(t, &c);
//    tree_rmv_object(t, &f);

    printf("\n\n");
    tree_print(t, print);

//    tree_delete(t);
//    free(a);
//    tree_print(t, print);
}

void test_container_3()
{
    int *ar = (int *)malloc(10 * 4);
    container *c = container_new_as_warapper(10, 4, ar);
    // container_add_element(c, ar);
    printf("c->logic_size: %d, c->alloc_size: %d, c->is_wrapper: %d\n", c->logic_size, c->alloc_size, c->is_wrapper);
    free(ar);
    container_delete(c);
}

void free_obj(void *o)
{
    object *obj = *(object **)o;
    printf("free o: %p, x: %d y: %d\n", obj, obj->x, obj->y);
    free(obj);
}

void test_container_2()
{
    int alloc_size = 1;
    int elem_size = sizeof(object *);

    container *c = container_new(alloc_size, elem_size, free_obj);

    object *o1 = (object *)malloc(sizeof(object));
    object *o2 = (object *)malloc(sizeof(object));

    o1->x = 1;
    o1->y = 2;
    printf("o1: %p, x: %d y: %d\n", o1, o1->x, o1->y);

    o2->x = 3;
    o2->y = 4;
    printf("o2: %p, x: %d y: %d\n", o2, o2->x, o2->y);

    container_add_element(c, &o1);
    container_add_element(c, &o2);

    void *e = container_remove_last_element(c);
    printf("e: %p\n", *(void **)e);

    container_delete(c);
    // free(o1);
    // free(o2);
}

void test_container_1()
{
    int alloc_size = 1;
    int elem_size = sizeof(int);
    container *c = container_new(alloc_size, elem_size, NULL);

    printf("c->alloc_size: %d\n", c->alloc_size);
    printf("c->logic_size: %d\n", c->logic_size);
    printf("c->elem_size: %d\n", c->elem_size);

    int elems[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    container_add_elements(c, (void *)&elems[0], 4);

    printf("====\n");
    printf("c->alloc_size: %d\n", c->alloc_size);
    printf("c->logic_size: %d\n", c->logic_size);
    printf("c->elem_size: %d\n", c->elem_size);

    container_add_elements(c, (void *)&elems[0], 2);

    printf("====\n");
    printf("c->alloc_size: %d\n", c->alloc_size);
    printf("c->logic_size: %d\n", c->logic_size);
    printf("c->elem_size: %d\n", c->elem_size);

    void *elem = container_remove_last_element(c);
    printf("====\n");
    printf("c->alloc_size: %d\n", c->alloc_size);
    printf("c->logic_size: %d\n", c->logic_size);
    printf("c->elem_size: %d\n", c->elem_size);
    printf("elem: %d\n", *(int *)elem);

    void *elems_r = container_remove_last_elements(c, 100);
    printf("====\n");
    printf("c->alloc_size: %d\n", c->alloc_size);
    printf("c->logic_size: %d\n", c->logic_size);
    printf("c->elem_size: %d\n", c->elem_size);
    if (elems_r)
    {
        printf("elem: %d\n", ((int *)elems_r)[0]);
        printf("elem: %d\n", ((int *)elems_r)[1]);
    }
    else
    {
        printf("elem: %p\n", elems_r);
    }

    container_delete(c);
}
