#ifndef __INTERFACE_H__
#define __INTERFACE_H__

typedef struct icomparable
{
    /*
        compare to objects and return -1, 0, or 1:
        -1: o1 is less than o2,
        0:  o1 equals to o2,
        1:  o1 is greater than o2
    */
    int (*compare_fn)(void *o1, void *o2);
} icomparable;

typedef struct ienumerable
{ /*
      returns 1 if next elemt exists otherwise 0
  */
    int (*next_fn)(void);

    /*
        returns a pointr to current element
    */
    void *(*current)(void);

    /*
        reset enumerator
    */
    void (*reset)(void);
} ienumerable;

typedef struct imoveable
{
    /*
        create a new obj
    */
    void* (*create_fn)(void* arg, int arg_size);

    /*
        delete existed object
    */
    void (*delete_fn)(void* );

    /*
        copy existed object and return a pointer of the copy
    */
    void* (*copy_fn)(void* );

} imoveable;

#endif /* __INTERFACE_H__ */