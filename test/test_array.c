#include "array.h"
#include "array_type_private.h"

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
	int *a = (int *)o;
    free(a);
}
int cmp(const void *o1, const void *o2)
{
    int *o_1 = (int *)o1;
    int *o_2 = (int *)o2;
    if(*o_1 == *o_2)
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
    object *obj = malloc(10 * sizeof(object));
    obj[0].x = 4;
    obj[0].y = 3;
    obj[1].x = 1;
    obj[1].y = 8;
    obj[2].x = 2;
    obj[2].y = 6;
    obj[5].x = 123;
    obj[5].y = 15;
    obj[6].x = 45;
    obj[6].y = 189;

    object *o = malloc(sizeof(object));
    o->x = 11;
    o->y = 12;

    int alloc_size = 1;
    int elem_size = sizeof(object);

    array_t *a = array_create(alloc_size, elem_size, NULL, test_array_dealloc_fn, cmp, 2, NULL);

    array_insert(a, (void *)&obj[0], 0, 3);
    array_push_back(a, &obj[1]);
    array_push_back(a, o);
    printf("obj: %p, a->data: %p\n", obj, *(object **)a->data);

    object *c = *(object **)array_get_data(a, 0);
    printf("%p, %p\n", c, obj[0]);

    printf("index: %d\n",array_get_index(a, o));
    array_rmv_element(a, 0);

    array_insert(a, (void *)&obj[5], 2, 2);

    array_rmv_element(a, 2);

    array_print(a, test_array_print);

    //array_delete(a);
}
