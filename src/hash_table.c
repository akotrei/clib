#include "hash_table.h"
#include "list.h"
#include "array.h"
#include "allocator_std.h"
#include "hash_table_type_private.h"
#include <assert.h>
#include <stdio.h>

int (*compare_keys)(void *, void *);

/*
 * Functions @key_copy_fake, @value_copy_fake that returns a pointer to the
 * user object if @key_copy_fn, @value_copy_fn is equal NULL
 *
 * @o    - pointer to user object
 */
static void* key_copy_fake(void *o) { return o; }
static void* val_copy_fake(void *o) { return o; }

/* Functions that does not delete anything if @key_dealloc_fn,
 * @value_dealloc_fn is NULL
 *
 * @o    - pointer to user object
 */
static void key_dealloc_fake(void *o) {}
static void val_dealloc_fake(void *o) {}

static pair_t*
pair_create(void *key,
            void *value,
            iallocator_t *iallocator);

static void
pair_delete();

static int compar(void *pair, void *key);

hash_table_t*
hash_table_create(void *buffer,
                  int table_size,
                  uint64_t (*hash_fn)(void *),
                  int (*compare_fn)(void *, void *),
                  void* (*key_copy_fn)(void *),
                  void* (*val_copy_fn)(void *),
                  void (*key_dealloc_fn)(void *),
                  void (*val_dealloc_fn)(void *),
                  int factor,
                  iallocator_t *iallocator)
{
    assert(table_size > 0 && "@table_size must be > 0");
    assert(iallocator != NULL && "@iallocator mustn't be equal NULL");
    assert(buffer != NULL && "@buffer mustn't be equal NULL");

    hash_table_t *ht = (hash_table_t *)buffer;

    ht->pair = NULL;
    ht->table_size = table_size;
    ht->factor = factor;
    ht->hash_fn = hash_fn;

    ht->compare_fn = compar;
    compare_keys = compare_fn;

    if(key_copy_fn != NULL)
        ht->key_copy_fn = key_copy_fn;
    else
        ht->key_copy_fn = key_copy_fake;

    if(val_copy_fn != NULL)
        ht->val_copy_fn = val_copy_fn;
    else
        ht->val_copy_fn = val_copy_fake;

    if(key_dealloc_fn != NULL)
        ht->key_dealloc_fn = key_dealloc_fn;
    else
        ht->key_dealloc_fn = key_dealloc_fake;

    if(val_dealloc_fn != NULL)
        ht->val_dealloc_fn = val_dealloc_fn;
    else
        ht->val_dealloc_fn = val_dealloc_fake;

    ht->iallocator = iallocator;

    ht->array = array_create(iallocator->allocate(NULL, array_sizeof()),
                             table_size,
                             sizeof(list_t *),
                             NULL,
                             NULL,
                             NULL,
                             2,
                             iallocator);

    for(int i = 0; i < table_size; i++)
    {
        list_t *l = list_create(iallocator->allocate(NULL, list_sizeof()),
                                NULL,
                                NULL,
                                compar,
                                iallocator);
        array_push_back(ht->array, &l);
    }

    return ht;
}

void
hash_table_delete(hash_table_t *ht)
{
    char *tmp = (char *)ht->array->data;
    iallocator_t* iallocator = ht->iallocator;
    for(int i = 0; i < ht->table_size; i++)
    {
        list_delete(*(list_t **)tmp);
        tmp += ht->array->elem_size;
    }
    array_delete(ht->array);
    iallocator->deallocate(NULL, ht);
}

/*
 * @hash_table_sizeof function returns size in the bytes of the structure
 * hash table
 */
int
hash_table_sizeof()
{
    return sizeof(hash_table_t);
}

void
hash_table_insert(hash_table_t *ht,
                  void *key,
                  void *value)
{
    list_add_tail(*(list_t **)array_get_data(ht->array, ht->hash_fn(key) % ht->table_size),
                  pair_create(key, value, ht->iallocator));
}

void*
hash_table_search(hash_table_t *ht,
                  void *key)
{

}

void*
hash_table_remove(hash_table_t *ht,
                  void *key)
{

}

void
hash_table_rehashing(hash_table_t *ht)
{

}

static pair_t*
pair_create(void *key,
            void *value,
            iallocator_t *iallocator)
{
    pair_t *pair = (pair_t *)iallocator->allocate(iallocator, sizeof(pair_t));
    pair->key = key;
    pair->value = value;
    return pair;
}

static int compar(void *pair, void *key)
{
    pair_t *p = (pair_t *)pair;
    return compare_keys(p->key, key);
}
