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

typedef struct worker 
{
	int id;
	char *first_name;
	char *name;
	char *surname;
	int sex;
	int age;
	double efficiency;
	double arr[1000];
	double arrr[1000000];
} worker;

void test_array_1();
void test_array_print(void *o);
void test_array_print(void *o)
{
    int *a = (int *)o;
    printf("%d", *a);
}
void test_array_dealloc_fn(void *o)
{
	int *a = (int *)o;
    free(a);
}

typedef struct obj_1
{
    int x;
    int y;
} obj_1;

int test_1();
void test_1_dealloc(void*);

int main(int argc, char **args)
{
    // test_array_1();
    test_1() == 0 ? printf("test_1 has been passed\n") :
                    printf("test_1 has not been passed\n"); 
    return 0;
}

void test_1_dell_arr(void* obj)
{
    array_t* ar = *(array_t**)obj;
    array_delete(ar);
}

int test_1()
{
    obj_1* o1 = malloc(sizeof(obj_1));
    o1->x = 5;
    o1->y = 9;

    array_t* array = array_create(2, 1, sizeof(obj_1*),
                                  NULL, test_1_dealloc,
                                  NULL, NULL);
    array_push_back(array, &o1);
    char* o1_extracted = array->data;
    printf("obj_1: %d %d extracted: %d %d\n", ((obj_1*)o1)->x,
                                              ((obj_1*)o1)->y,
                                              (*(obj_1**)o1_extracted)->x,
                                              (*(obj_1**)o1_extracted)->y);
    
    array_t* array_base = array_create(2, 1, sizeof(array_t*),
                                       NULL, test_1_dell_arr,
                                       NULL, NULL);

    array_push_back(array_base, &array);

    printf("o1: %p\n", o1);
    array_delete(array_base);
    return 0;
}

void test_1_dealloc(void* obj)
{
    obj_1* o = *(obj_1**)obj;
    printf("o: %d %d\n", o->x, o->y);
    printf("free: %p\n", *(void**)obj);
    free(*(void**)obj);
}

void test_array_1()
{
    int *b = malloc(sizeof(int)); *b = 1;
    int *r = malloc(sizeof(int)); *r = 2;
    int *g = malloc(sizeof(int)); *g = 6;
	worker *value = malloc(sizeof(worker));
	printf("%ld\n", sizeof(worker));
    int alloc_size = 1;
    int elem_size = sizeof(worker);
    array_t *a = array_create(2, alloc_size, elem_size, NULL, test_array_dealloc_fn, NULL, NULL);

	array_push_back(a, b);
	array_push_back(a, r);
	array_push_back(a, r);
	array_push_back(a, r);
	array_push_front(a, r);
	array_push_front(a, g);
	array_push_front(a, g);
	array_push_front(a, g);

	for(int i = 0; i < 100000000; i++)
		array_push_back(a, value);

//    array_add_obj(a, b);
//    array_add_obj(a, r);
//    int *l = (int *)array_fnd_obj(a, r);
/*    void *c = (void *)a->data;
    int *k = (int *)c;
    printf("%d\n", *k);*/
//    array_print(a, test_array_print);

    array_delete(a);
	free(b);
	free(r);
	free(g);
}
