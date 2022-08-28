#include "binary_search_tree.h"
#include "binary_search_tree_type_private.h"
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
int test_2();
int test_3();
int test_4();
int test_5();
int test_6();
int test_7();
int test_8();

int main(int argc, char **args)
{
    printf("======================================================= TEST 1 =======================================================\n");
    printf("SCRIPT DESCRIPTION\n");
    printf("Creating a tree without using the functions of copying and deleting objects,\n");
    printf("only the function of comparing objects, since this is the main function for working with a tree,\n");
    printf("so that you can fill the tree with objects. Testing adding an object to a tree, deleting a tree.\n\n");
    test_1() == 0 ? printf("TEST 1 HAS BEEN PASSED\n") : printf("TEST 1 HASN'T BEEN PASSED\n");
    printf("======================================================================================================================\n\n");

    printf("======================================================= TEST 2 =======================================================\n");
    printf("SCRIPT DESCRIPTION\n");
    printf("Creating a tree using the comparison function (required) and deleting the object.\n");
    printf("Testing the functions of adding an object and deleting a tree. Since the function of deleting an object will be passed when\n");
    printf("creating a tree, the result of deleting an object when deleting a tree is expected,\n");
    printf("which will subsequently free from deleting an object outside the tree, that is, calling a free function  is not required.\n\n");
    test_2() == 0 ? printf("TEST 2 HAS BEEN PASSED\n") : printf("TEST 2 HASN'T BEEN PASSED\n");
    printf("======================================================================================================================\n\n");

    printf("======================================================= TEST 3 =======================================================\n");
    printf("SCRIPT DESCRIPTION\n");
    printf("Creating a tree using the comparison function (required) and deleting the object.\n");
    printf("Testing the functions of adding an object and deleting a tree. Since the function of deleting an object will be passed when\n");
    printf("creating a tree, the result of deleting an object when deleting a tree is expected,\n");
    printf("which will subsequently free from deleting an object outside the tree, that is, calling a free function  is not required.\n\n");
    test_3() == 0 ? printf("TEST 3 HAS BEEN PASSED\n") : printf("TEST 3 HASN'T BEEN PASSED\n");
    printf("======================================================================================================================\n\n");

    printf("======================================================= TEST 4 =======================================================\n");
    printf("SCRIPT DESCRIPTION\n");
    printf("creating a tree using the object comparison functions and the function of copying an\n");
    printf("object pointer to a tree, adding an object pointer to the tree, and deleting the tree;\n");
    printf("note: when deleting the tree, the copied pointer will not be deleted, since when it was created,\n");
    printf("the object deletion function was not passed.\n\n");
    test_4() == 0 ? printf("TEST 4 HAS BEEN PASSED\n") : printf("TEST 4 HASN'T BEEN PASSED\n");
    printf("======================================================================================================================\n\n");

    printf("======================================================= TEST 5 =======================================================\n");
    printf("SCRIPT DESCRIPTION\n");
    printf("Creating a tree with only object comparison function. Testing the functions of adding,\n");
    printf("deleting, searching and deleting objects, as well as checking the tree balancing algorithm.\n\n");
    test_5() == 0 ? printf("TEST 5 HAS BEEN PASSED\n") : printf("TEST 5 HASN'T BEEN PASSED\n");
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
    tree_t *t = NULL;
    t = tree_create(cmp_fn,
                    NULL,
                    NULL,
                    NULL);

    t == NULL ? assert("@t tree wasn't created") : printf("address of the created tree: [%p]\n", t);

    t->compare_fn == NULL ? assert("@compare_fn function in the tree is equal NULL") : 
                            printf("the address of the passed compare function and the compare function inside the tree: [%p], [%p]\n", cmp_fn, t->compare_fn);

    student *s1 = NULL;
    s1 = (student *)malloc(sizeof(student));
    s1->id = 1922290;
    s1->name = "Ivan";
    s1->first_name = "Ivanov";
    s1->last_name = "Ivanovich";
    s1->age = 20;
    s1->sex = 'M';
    s1->mark = 7.0;

    s1 == NULL ? assert("@s1 object wasn't created") : printf("the address of the created item @s1 of the structure student: [%p]\n", s1);

    printf("adding to the tree the address [%p] of the pointer(&s1) of the created object @s1 to the tree\n", &s1);
    tree_add_object(t, &s1);

    printf("address of pointer to data in root knot: [%p], address of pointer to object @s1: [%p]\n", t->knot->data, &s1);

    printf("address of data in root knot: [%p], address of object @s1: [%p]\n", *(char **)t->knot->data, s1);

    printf("\nprinting to the console the contents of the object accessing it through a pointer that is stored in the tree:\n");
    print_fn(t->knot->data);
    printf("\n");

    printf("calling the tree deletion function, but when deleting the tree, the object\n");
    printf("will not be deleted because the pointer to the object deletion function was not passed\n");
    tree_delete(t);

    printf("\naddress of the object @s1 to be deleted created at the beginning of the test to be added to the tree: [%p]\n", s1);
    free(s1);
    printf("\n");

    return 0;
}

int test_2()
{
    tree_t *t = NULL;
    t = tree_create(cmp_fn,
                    NULL,
                    dealloc_fn,
                    NULL);

    t == NULL ? assert("@t tree wasn't created") : printf("address of the created tree: [%p]\n", t);

    t->compare_fn == NULL ? assert("@compare_fn function in the tree is equal NULL") : 
                            printf("the address of the passed compare function and the compare function inside the tree: [%p], [%p]\n", cmp_fn, t->compare_fn);

    t->dealloc_fn == NULL ? assert("@dealloc_fn function in the tree is equal NULL") : 
                            printf("the address of the passed deallocate function and the deallocate function inside the tree: [%p], [%p]\n", dealloc_fn, t->dealloc_fn);

    student *s1 = NULL;
    s1 = (student *)malloc(sizeof(student));
    s1->id = 1922290;
    s1->name = "Ivan";
    s1->first_name = "Ivanov";
    s1->last_name = "Ivanovich";
    s1->age = 20;
    s1->sex = 'M';
    s1->mark = 7.0;

    s1 == NULL ? assert("@s1 object wasn't created") : printf("the address of the created item @s1 of the structure student: [%p]\n", s1);

    printf("adding to the tree the address [%p] of the pointer(&s1) of the created object @s1 to the tree\n", &s1);
    tree_add_object(t, &s1);

    printf("address of pointer to data in root knot: [%p], address of pointer to object @s1: [%p]\n", t->knot->data, &s1);
    printf("address of data in root knot: [%p], address of object @s1: [%p]\n", *(char **)t->knot->data, s1);
    printf("\nprinting to the console the contents of the object accessing it through a pointer that is stored in the tree:\n");
    print_fn(t->knot->data);
    printf("\n");

    printf("calling the tree deletion function, when the tree is deleted, the object itself will also be deleted,\n");
    printf("since when creating the tree, a pointer to the object deletion function was passed to it,\n");
    printf("that is, deleting an object outside the tree is not required\n");
    tree_delete(t);

    return 0;
}

int test_3()
{
    tree_t *t = NULL;
    t = tree_create(cmp_fn,
                    pointer_copy_fn,
                    pointer_dealloc,
                    NULL);

    t == NULL ? assert("@t tree wasn't created") : printf("address of the created tree: [%p]\n", t);

    t->compare_fn == NULL ? assert("@compare_fn function in the tree is equal NULL") : 
                            printf("the address of the passed compare function and the compare function inside the tree: [%p], [%p]\n", cmp_fn, t->compare_fn);

    t->dealloc_fn == NULL ? assert("@dealloc_fn function in the tree is equal NULL") : 
                            printf("the address of the passed deallocate function and the deallocate function inside the tree: [%p], [%p]\n", pointer_dealloc, t->dealloc_fn);

    t->copy_fn == NULL ? assert("@copy_fn function in the tree is equal NULL") : 
                            printf("the address of the passed copy pointer function and the copy pointer function inside the tree: [%p], [%p]\n", pointer_copy_fn, t->copy_fn);

    student *s1 = NULL;
    s1 = (student *)malloc(sizeof(student));
    s1->id = 1922290;
    s1->name = "Ivan";
    s1->first_name = "Ivanov";
    s1->last_name = "Ivanovich";
    s1->age = 20;
    s1->sex = 'M';
    s1->mark = 7.0;

    s1 == NULL ? assert("@s1 object wasn't created") : printf("the address of the created item @s1 of the structure student: [%p]\n", s1);

    printf("adding to the tree the address [%p] of the pointer(&s1) of the created object @s1 to the tree\n", &s1);
    tree_add_object(t, &s1);

    printf("address of copied pointer to data in root knot: [%p], address of pointer to object @s1: [%p]\n", t->knot->data, &s1);
    printf("address of data in root knot: [%p], address of object @s1: [%p]\n", *(char **)t->knot->data, s1);
    printf("\nprinting to the console the contents of the object accessing it through a pointer that is stored in the tree:\n");
    print_fn(t->knot->data);
    printf("\n");

    printf("calling the tree deletion function, along with deleting the tree, the copied pointer\n");
    printf("to the tree nodes, which points to the pointer that points to the user object, will be deleted;\n");
    printf("note: depending on the implementation of the delete function, it is possible to\n");
    printf("delete both the object via the copied pointer and the pointer itself.\n");
    tree_delete(t);

    free(s1);

    return 0;
}

int test_4()
{
    tree_t *t = NULL;
    t = tree_create(cmp_fn,
                    pointer_copy_fn,
                    NULL,
                    NULL);

    t == NULL ? assert("@t tree wasn't created") : printf("address of the created tree: [%p]\n", t);

    t->compare_fn == NULL ? assert("@compare_fn function in the tree is equal NULL") : 
                            printf("the address of the passed compare function and the compare function inside the tree: [%p], [%p]\n", cmp_fn, t->compare_fn);

    t->copy_fn == NULL ? assert("@copy_fn function in the tree is equal NULL") : 
                            printf("the address of the passed copy pointer function and the copy pointer function inside the tree: [%p], [%p]\n", pointer_copy_fn, t->copy_fn);

    student *s1 = NULL;
    s1 = (student *)malloc(sizeof(student));
    s1->id = 1922290;
    s1->name = "Ivan";
    s1->first_name = "Ivanov";
    s1->last_name = "Ivanovich";
    s1->age = 20;
    s1->sex = 'M';
    s1->mark = 7.0;

    s1 == NULL ? assert("@s1 object wasn't created") : printf("the address of the created item @s1 of the structure student: [%p]\n", s1);

    printf("adding to the tree the address [%p] of the pointer(&s1) of the created object @s1 to the tree\n", &s1);
    tree_add_object(t, &s1);

    printf("address of copied pointer to data in root knot: [%p], address of pointer to object @s1: [%p]\n", t->knot->data, &s1);
    printf("address of data in root knot: [%p], address of object @s1: [%p]\n", *(char **)t->knot->data, s1);
    printf("\nprinting to the console the contents of the object accessing it through a pointer that is stored in the tree:\n");
    print_fn(t->knot->data);
    printf("\n");

    printf("calling the delete function of the tree, which will delete everything except the copied object pointer\n");
    printf("to this tree, since no pointer to the object delete function was passed\n");
    tree_delete(t);

    printf("calling the object's deletion function, since it was not deleted by means of the tree\n");
    free(s1);

    return 0;
}

int test_5()
{
    tree_t *t = NULL;
    t = tree_create(any_cmp_fn,
                    NULL,
                    NULL,
                    NULL);

    t == NULL ? assert("@t tree wasn't created") : printf("address of the created tree: [%p]\n", t);

    t->compare_fn == NULL ? assert("@compare_fn function in the tree is equal NULL") : 
                            printf("the address of the passed compare function and the compare function inside the tree: [%p], [%p]\n", any_cmp_fn, t->compare_fn);

    point *p1 = malloc(sizeof(point));
    p1->x = 2; p1->y = 3; p1->z = 5;

    point *p2 = malloc(sizeof(point));
    p2->x = 3; p2->y = 5; p2->z = 7;

    point *p3 = malloc(sizeof(point));
    p3->x = 6; p3->y = 9; p3->z = 12;

    point *p4 = malloc(sizeof(point));
    p4->x = 7; p4->y = 10; p4->z = 15;

    point *p5 = malloc(sizeof(point));
    p5->x = 8; p5->y = 13; p5->z = 17;

    printf("address of created points: [%p], [%p], [%p], [%p], [%p]\n\n", p1, p2, p3, p4, p5);

    tree_add_object(t, p1);
    tree_add_object(t, p2);
    tree_add_object(t, p3);
    tree_add_object(t, p4);
    tree_add_object(t, p5);

    printf("print tree in console before balancing\n\n");
    tree_print(t, print_point);

    tree_balance_DSW(t);

    printf("\nprint tree in console after balancing\n");
    tree_print(t, print_point);
    printf("\n");

    printf("address of found object: [%p], address of object that need to be found: [%p]\n", tree_fnd_object(t, p3), p3);
    printf("address of removed object: [%p], address of object that need to be removed: [%p]\n", tree_rmv_object(t, p4), p4);

    printf("\nprint tree in console after found and removed operations\n");
    tree_print(t, print_point);
    printf("\n");

    tree_delete(t);
    free(p1);
    free(p2);
    free(p3);
    free(p4);
    free(p5);

    return 0;
}
