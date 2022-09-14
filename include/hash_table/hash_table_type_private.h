#ifndef __HASH_TABLE_TYPE_PRIVATE_H__
#define __HASH_TABLE_TYPE_PRIVATE_H__

#include <stdlib.h>
#include <stdint.h>
#include "array_type_public.h"
#include "array_type_private.h"

/*
 * Pair key-value type declaration
 */
typedef struct _pair_t pair_t;

/*
 * Pair key-value type definition
 */
typedef struct _pair_t
{
    /* Pointer to the key*/
    void *key;

    /* Pointer to the value*/
    void *value;

} _pair_t;

/*
 * Hash table type definition
 */
typedef struct _hash_table_t
{
    /* Pointer to the pair key-value*/
    pair_t *pair;

    /* Hash table size*/
    int table_size;

    /* Pointer to a hash-function*/
    uint64_t (*hash_fn)(void *);

    /* Pointer to the array*/
    array_t *array;

    /* Pointer to the compare function*/
    int (*compare_fn)(void *, void *);

    /* Functions for copying and deleting keys and values*/
    void* (*key_copy_fn)(void *);
    void* (*val_copy_fn)(void *);
    void (*key_dealloc_fn)(void *);
    void (*val_dealloc_fn)(void *);

    /* Size of hash table rehashing*/
    int factor;

    /* Pointer to the interface of the allocator*/
    iallocator_t *iallocator;

} _hash_table_t;

#endif /* __HASH_TABLE_TYPE_PRIVATE_H__ */
