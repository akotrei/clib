#include "container.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int test_tree_cmp_fn_obj(void *obj1, void *obj2);
void test_tree_1();
void test_tree_2();
void test_tree_delete(void *);
void print(void *o);
void* copy_fn(void *o);
void print_obj(void *o);

int main(int argc, char **args)
{
//    test_container_1();
//    test_container_2();
//    test_container_3();
    test_tree_1();
    test_tree_2();
    return 0;
}

void print(void *o)
{
    if(o != NULL)
    {
        int *a = (int *)o;
        printf("[%d]", *a);
    }
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

void* copy_fn(void *o)
{
    int *tmp = (int *)o;
    void *data = malloc(sizeof(int));
    memcpy(data, tmp, sizeof(int));
    return data;
}

void test_tree_1()
{
    int *a1 = malloc(sizeof(int)); *a1 = 5;
    int *a2 = malloc(sizeof(int)); *a2 = 4;
    int *a3 = malloc(sizeof(int)); *a3 = 3;
    int *a4 = malloc(sizeof(int)); *a4 = 7;
    int *a5 = malloc(sizeof(int)); *a5 = 8;
    int *a6 = malloc(sizeof(int)); *a6 = 2;
    int *a7 = malloc(sizeof(int)); *a7 = 1;
    tree_t *t1 = tree_create(test_tree_cmp_fn, copy_fn, test_tree_delete, NULL);

    tree_add_object(t1, a1);
    tree_add_object(t1, a2);
    tree_add_object(t1, a3);
    tree_add_object(t1, a4);
    tree_add_object(t1, a5);
    tree_add_object(t1, a6);
    tree_add_object(t1, a7);
//    tree_add_object(t1, f);

    tree_print(t1, print);
    
    int *r3 = (int*)tree_rmv_object(t1, a2);
    int *r4 = (int*)tree_rmv_object(t1, a4);
    printf("r3 = %d, r4 = %d\n", *r3, *r4);
    printf("\n");
    tree_print(t1, print);
//    tree_balance_DSW(t1);
//    printf("\n\n");
//    tree_print(t1, print);

    printf("%d\n", *a1);
    printf("%d\n", *a2);
    printf("%d\n", *a3);
    printf("%d\n", *a4);
    printf("%d\n", *a5);
    printf("%d\n", *a6);
    printf("%d\n", *a7);

    tree_delete(&t1);
//    tree_print(t1, print);
//    free(a1);
}

void print_obj(void *o)
{
    object *obj = (object*)o;
    printf("(%d, %d)", obj->x, obj->y);
}

int test_tree_cmp_fn_obj(void *obj1, void *obj2)
{
    object *obj_1 = (object *)obj1;
    object *obj_2 = (object *)obj2;

    if(obj_1->x < obj_2->x)
        return 1;
    if(obj_1->x > obj_2->x)
        return -1;
    return 0;
}

void test_tree_2()
{
    tree_t *t1 = tree_create(test_tree_cmp_fn_obj, NULL, NULL, NULL);
    object *o1 = (object *)malloc(sizeof(object));
    object *o2 = (object *)malloc(sizeof(object));
    object *o3 = (object *)malloc(sizeof(object));
    object *o4 = (object *)malloc(sizeof(object));
    object *o5 = (object *)malloc(sizeof(object));

    o1->x = 5;
    o1->y = 3;
    printf("o1: %p, x: %d y: %d\n", o1, o1->x, o1->y);

    o2->x = 6;
    o2->y = 6;
    printf("o1: %p, x: %d y: %d\n", o2, o2->x, o2->y);

    o3->x = -3;
    o3->y = 643;
    printf("o1: %p, x: %d y: %d\n", o3, o3->x, o3->y);

    o4->x = 2;
    o4->y = 678;
    printf("o1: %p, x: %d y: %d\n", o4, o4->x, o4->y);

    o5->x = 10;
    o5->y = 41;
    printf("o1: %p, x: %d y: %d\n", o5, o5->x, o5->y);
    

    tree_add_object(t1, o1);
    tree_add_object(t1, o2);
    tree_add_object(t1, o3);
    tree_add_object(t1, o4);
    tree_add_object(t1, o5);

    tree_print(t1, print_obj);

    
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
