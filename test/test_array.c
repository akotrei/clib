#include "array.h"
#include "array_type_private.h"
#include "allocator_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct object
{
    int x;
    int y;
} object;

void test_array_1();
void test_array_print(void *o);
void test_array_print(void *o)
{
    object *a = (object *)o;
    printf("(%d, %d)\n", a->x, a->y);
}
void test_array_dealloc_fn(void *o)
{
	object *a = (object *)o;
    free(a);
}
int cmp(const void *o1, const void *o2)
{
    object *o_1 = (object *)o1;
    object *o_2 = (object *)o2;
    if(o_1->x == o_2->x)
        return 0;
    return -1;
}

int main(int argc, char **args)
{
    test_array_1();
    return 0;
}

void test_array_1()
{

    object *o = malloc(sizeof(object));
    o->x = 11;
    o->y = 12;

    object *o1 = malloc(sizeof(object));
    o1->x = 1;
    o1->y = 3;

    char mem[sizeof(iallocator_t)];
    iallocator_t *iallocator = allocator_std_new(mem);

    int alloc_size = 1;
    int elem_size = sizeof(object);

    array_t *a0 = array_create(iallocator->allocate(NULL, array_sizeof()),
                               alloc_size,
                               elem_size,
                               NULL,
                               NULL,
                               cmp,
                               2,
                               iallocator);

    array_t *a = array_create(iallocator->allocate(NULL, array_sizeof()),
                              alloc_size,
                              elem_size,
                              NULL,
                              NULL,
                              cmp,
                              2,
                              iallocator);

    array_push_back(a, o);
    array_push_back(a, o1);
    printf("address of @o: %p, address of data in the array: %p\n", o, a->data);
    test_array_print(o);
    //test_array_print(array_get_data(a, 0));
    //test_array_print(array_get_data(a, 1));
    printf("array size: %d\n", array_size(a));
    printf("index: %d\n", array_get_index(a, o1));

    array_delete(a);
    array_delete(a0);
    free(o);
    free(o1);

/*    array_insert(a, (void *)&obj[0], 0, 3);
    array_push_back(a, &obj[1]);
    array_push_back(a, o);
    printf("obj: %p, a->data: %p\n", &obj[0], (char **)a->data);

    object *c = *(object **)array_get_data(a, 0);
    printf("%p, %p\n", c, obj[0]);

    printf("index: %d\n",array_get_index(a, o));
    array_rmv_element(a, 0);

    array_insert(a, (void *)&obj[5], 2, 2);
    array_push_back(a, &o);

    printf("obj: %p, a->data: %p\n", o, *(object **)array_get_data(a, array_get_index(a, &o)));

    array_rmv_element(a, 2);

    printf("size of array: %d\n", array_size(a));

    array_print(a, test_array_print);

    array_delete(a);*/
}
