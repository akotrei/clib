#ifndef __HASH_TABLE_TYPE_PRIVATE_H__
#define __HASH_TABLE_TYPE_PRIVATE_H__

#include <stdlib.h>
#include <stdint.h>
#include "array_type_public.h"
#include "array_type_private.h"

/*
 * pair key-value type declaration
 */
typedef struct _pair_t
{
    void *key;
    void *value;
} _pair_t;

/*
 * hash table type declaration
 */
typedef struct _hash_table_t
{
    pair_t *pair;
    int iallocator_owner;
    int table_size;
    uint64_t (*hash_fn)(const void *);
    array_t *array;
    int (*compare_fn)(void *, void *);
    void* (*copy_fn)(void *);
    void (*dealloc_fn)(void *);
    int factor;
    iallocator_t *iallocator;
} _hash_table_t;

#endif /* __HASH_TABLE_TYPE_PRIVATE_H__ */
