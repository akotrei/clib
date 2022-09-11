#include "list.h"
#include "list_type_private.h"
#include "list_iterator.h"
#include "allocator_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct object
{
    int x;
    int y;
    char *color;
} object;

int compar_fn(void *o1, void *o2);
void* copy_obj_fn(void *o);
void dealloc_obj_fn(void *o);
void print_obj_fn(void *o);
int test_1();
int test_2();
int test_3();

int main(int argc, char **args)
{
    printf("============================================================== TEST 1 ==============================================================\n");
    printf("SCRIPT DESCRIPTION\n");
    printf("Testing the functions of creating a list, deleting a list, adding an object to a list.\n");
    printf("The case of adding a list to a list, adding an object to the added list is considered.\n\n");
    test_1() == 0 ? printf("TEST 1 HAS BEEN PASSED\n") : printf("TEST 1 HASN'T BEEN PASSED\n");
    printf("====================================================================================================================================\n\n");

    /*printf("============================================================== TEST 2 ==============================================================\n");
    printf("SCRIPT DESCRIPTION\n");
    printf("Testing the functions of creating and deleting a list, adding an object to a list,\n");
    printf("copying an object, deleting an object, and the object comparison function\n\n");
    test_2() == 0 ? printf("TEST 2 HAS BEEN PASSED\n") : printf("TEST 2 HASN'T BEEN PASSED\n");
    printf("====================================================================================================================================\n\n");

    printf("============================================================== TEST 3 ==============================================================\n");
    printf("SCRIPT DESCRIPTION\n");
    printf("Testing the functions of creating and deleting a list, adding an object\n");
    printf("to the list at its tail and head, deleting, searching and adding starting\n");
    printf("from the head and tail of the list, changing objects in places in the\n");
    printf("nodes of the list, getting the size of the list and deleting all nodes in\n");
    printf("the list (make the list empty) .\n\n");
    test_3() == 0 ? printf("TEST 3 HAS BEEN PASSED\n") : printf("TEST 3 HASN'T BEEN PASSED\n");
    printf("====================================================================================================================================\n\n");*/
    return 0;
}

int compar_fn(void *o1, void *o2)
{
    object *o_1 = (object *)o1;
    object *o_2 = (object *)o2;
    if(strcmp(o_1->color, o_2->color) == 0)
        return 0;
    return -1;
}

void* copy_obj_fn(void *o)
{
    void *result = malloc(sizeof(object));
    memcpy(result, o, sizeof(object));
    return result;
}

void dealloc_obj_fn(void *o)
{
    object *_o = (object *)o;
    free(_o->color);
    free(_o);
}

void print_obj_fn(void *o)
{
    object *_o = (object *)o;
    printf("[color: %s, x: %d, y: %d]", _o->color, _o->x, _o->y);
}

int test_1()
{
    char mem[sizeof(iallocator_t)];
    iallocator_t *iallocator = allocator_std_new(mem);

    char iter_mem[sizeof(ilist_iterator_t)];
    ilist_iterator_t *ilist_iterator = list_iterator_create(iter_mem);

    list_t *l = list_create(iallocator->allocate(NULL, list_sizeof()),
                            NULL,
                            NULL,
                            NULL,
                            iallocator);

    object *o = malloc(sizeof(object));
    o->x = 5;
    o->y = 3;
    o->color = malloc(4 * sizeof(char));
    o->color[0] = 'r';
    o->color[1] = 'e';
    o->color[2] = 'd';
    o->color[3] = '\0';

    object *o1 = malloc(sizeof(object));
    o1->x = 21;
    o1->y = 34;
    o1->color = malloc(4 * sizeof(char));
    o1->color[0] = 'r';
    o1->color[1] = 'e';
    o1->color[2] = 'd';
    o1->color[3] = '\0';

    list_add_head(l, o);
    list_add_tail(l, o1);
    print_obj_fn(l->head->next->data);
    printf("\nlist size: %d\n", list_size(l));
    //list_clear(l);
    printf("\nlist size: %d\n", list_size(l));

    printf("%p\n", l->head->data);
    ilist_iterator->next(l);
    ilist_iterator->prev(l);
    ilist_iterator->begin(l);

    printf("%p\n", ilist_iterator->get_data(ilist_iterator->curr(l)));

    list_delete(l);

    free(o->color);
    free(o);

    free(o1->color);
    free(o1);

    return 0;
}

/*int test_2()
{
    list_t *l = NULL;
    l = list_create(copy_obj_fn, dealloc_obj_fn, compar_fn, NULL);

    l == NULL ? assert("@l list wasn't created\n") : printf("address of the created tree: [%p]\n", l);

    l->compare_fn == NULL ? assert("@compare_fn function in the tree is equal NULL") :
                            printf("the address of the passed compare function and the compare function inside the list: [%p], [%p]\n", compar_fn, l->compare_fn);

    l->dealloc_fn == NULL ? assert("@dealloc_fn function in the tree is equal NULL") :
                            printf("the address of the passed deallocate function and the deallocate function inside the list: [%p], [%p]\n", dealloc_obj_fn, l->dealloc_fn);

    l->copy_fn == NULL ? assert("@copy_fn function in the tree is equal NULL") :
                            printf("the address of the passed copy pointer function and the copy pointer function inside the list: [%p], [%p]\n\n", copy_obj_fn, l->copy_fn);

    object *o1 = (object *)malloc(sizeof(object));
    o1->color = (char *)malloc(3 * sizeof(char));
    o1->color[0] = 'r';
    o1->color[1] = 'e';
    o1->color[2] = 'd';
    o1->x = 3;
    o1->y = 5;

    o1 == NULL ? assert("@o1 object wasn't created\n") : printf("the address of the created item @o1 of the structure object: [%p]\n\n", o1);

    list_add_tail(l, o1);

    printf("address of data in head node of the list: [%p], address of object @o1: [%p]\n", l->head->data, o1);

    printf("calling function deleting of list and will be deleted of copied object @o1: [%p]\n", l->head->data);
    list_delete(l);

    printf("free added object to the list: [%p]\n", o1);
    free(o1);

    return 0;
}

int test_3()
{
    list_t *l = NULL;
    l = list_create(copy_obj_fn, dealloc_obj_fn, compar_fn, NULL);

    l == NULL ? assert("@l list wasn't created\n") : printf("address of the created tree: [%p]\n", l);

    l->compare_fn == NULL ? assert("@compare_fn function in the tree is equal NULL") :
                            printf("the address of the passed compare function and the compare function inside the list: [%p], [%p]\n", compar_fn, l->compare_fn);

    l->dealloc_fn == NULL ? assert("@dealloc_fn function in the tree is equal NULL") :
                            printf("the address of the passed deallocate function and the deallocate function inside the list: [%p], [%p]\n", dealloc_obj_fn, l->dealloc_fn);

    l->copy_fn == NULL ? assert("@copy_fn function in the tree is equal NULL") :
                            printf("the address of the passed copy pointer function and the copy pointer function inside the list: [%p], [%p]\n\n", copy_obj_fn, l->copy_fn);

    object *o1 = (object *)malloc(sizeof(object));
    o1->color = (char *)malloc(4 * sizeof(char));
    o1->color[0] = 'r';
    o1->color[1] = 'e';
    o1->color[2] = 'd';
    o1->color[3] = '\0';
    o1->x = 3;
    o1->y = 5;

    o1 == NULL ? assert("@o1 object wasn't created\n") : printf("the address of the created item @o1 of the structure object: [%p]\n\n", o1);

    list_add_tail(l, o1);

    object *o2 = (object *)malloc(sizeof(object));
    o2->color = (char *)malloc(7 * sizeof(char));
    o2->color[0] = 'y';
    o2->color[1] = 'e';
    o2->color[2] = 'l';
    o2->color[3] = 'l';
    o2->color[4] = 'o';
    o2->color[5] = 'w';
    o1->color[6] = '\0';
    o2->x = 6;
    o2->y = 7;

    o2 == NULL ? assert("@o2 object wasn't created\n") : printf("the address of the created item @o2 of the structure object: [%p]\n\n", o2);

    printf("adding multiple objects to remove the search for them in the list\n\n");
    list_add_head(l, o2);
    list_add_head(l, o1);
    list_add_head(l, o1);
    list_add_head(l, o1);
    list_add_head(l, o2);

    object *fnd_obj = (object *)list_fnd_shead(l, o1);
    printf("print address of found object and its contents: [%p]\n", fnd_obj);
    print_obj_fn(fnd_obj);

    list_rmv_shead(l, o1);
    list_rmv_shead(l, o1);
    list_rmv_shead(l, o1);
    list_rmv_shead(l, o1);
    printf("remove all @o1 objects from the list and print list to the console\n");
    list_prt_shead(l, print_obj_fn);
    printf("\n");


    printf("calling function of clearing list(list will become empty)\n");
    list_clear(l);

    return 0;

}*/
