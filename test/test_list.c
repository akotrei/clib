#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct object
{
    int x;
    int y;
} object;

void test_list_1();
int test_list_cmp_fn(void *o1, void *o2);
void test_list_print(void *o);
void test_list_dealloc_fn(void *o);

int main(int argc, char **args)
{
    test_list_1();
    return 0;
}

void test_list_1()
{
    int *a = malloc(sizeof(int)); *a = 5;
    int *c = malloc(sizeof(int)); *c = 6;
    int *d = malloc(sizeof(int)); *d = 2;
    int *f = malloc(sizeof(int)); *f = 8;

    list_t *l = list_create(NULL, test_list_dealloc_fn, test_list_cmp_fn, NULL);

    list_add_head(l, a);
    list_add_head(l, c);
    list_add_tail(l, d);
    list_add_tail(l, f);



    int *b = (int *)l->tail->data;
    printf("b = %d\n", *b);

    list_prt_shead(l, test_list_print);
    list_prt_stail(l, test_list_print);

    printf("%p\n", l->tail->next);
    list_delete(l);
}

int test_list_cmp_fn(void *o1, void *o2)
{
    int *o_1 = (int*)o1;
    int *o_2 = (int*)o2;
    if(*o_1 == *o_2)
        return 0;
    return -1;
}

void test_list_print(void *o)
{
    int *a = (int*)o;
    printf("%d", *a);
}

void test_list_dealloc_fn(void *o)
{
    int *a = (int*)o;
    free(a);
}
