#include "hash_table.h"
#include "list.h"
#include "array.h"
#include "allocator_std.h"
#include "hash_table_type_private.h"
#include <assert.h>
#include <stdio.h>

int (*compare_keys)(void *, void *);

static pair_t* 
pair_create(void *key,
            void *value,
            iallocator_t *iallocator);

static void
pair_delete();

static int compar(void *pair, void *key);

hash_table_t* 
hash_table_create(int table_size,
                  uint64_t (*hash_fn)(const void *),
                  int (*compare_fn)(void *, void *),
                  void* (*copy_fn)(void *),
                  void (*dealloc_fn)(void *),
                  int factor,
                  iallocator_t *iallocator)
{
    assert(table_size > 0 && "@table_size must be > 0");
    assert(hash_fn != NULL && "@hash_fn mustn't be equal NULL");

    iallocator_t *_iallocator;

    allocator_std_t *iallocator_std = NULL;

    int _iallocator_owner;

    if(iallocator == NULL)
    {
        iallocator_std = allocator_std_new();

        _iallocator = allocator_std_get_allocator(iallocator_std);

        _iallocator_owner = 1;
    }
    else 
    {
        _iallocator = iallocator;

        _iallocator_owner = 0;
    }

    hash_table_t *ht = (hash_table_t *)_iallocator->allocate(_iallocator->self,
                                                             sizeof(hash_table_t));
    ht->pair = NULL;
    ht->iallocator_owner = _iallocator_owner;
    ht->table_size = table_size;
    ht->factor = factor;
    ht->hash_fn = hash_fn;

    ht->compare_fn = compar;
    compare_keys = compare_fn;
    ht->copy_fn = copy_fn;
    ht->dealloc_fn = dealloc_fn;
    ht->iallocator = _iallocator;

    ht->array = array_create(table_size, 
                             sizeof(list_t *),
                             copy_fn,
                             dealloc_fn,
                             NULL,
                             2,
                             NULL);

    for(int i = 0; i < table_size; i++)
    {
        list_t *l = list_create(copy_fn,
                                dealloc_fn,
                                compar,
                                NULL);
        array_push_back(ht->array, &l);
    }
    
    return ht;
}

void 
hash_table_delete(hash_table_t *ht)
{
    char *tmp = (char *)ht->array->data;
    for(int i = 0; i < ht->table_size; i++)
    {
        list_delete(*(list_t **)tmp);
        tmp += ht->array->elem_size;
    }
    array_delete(ht->array);
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
    return list_fnd_shead(*(list_t **)array_get_data(ht->array, 
                                                     ht->hash_fn(key) % ht->table_size),
                          key);
}

void* 
hash_table_remove(hash_table_t *ht, 
                  void *key)
{
    return list_rmv_shead(*(list_t **)array_get_data(ht->array,
                                                     ht->hash_fn(key) % ht->table_size),
                          key);
}

void 
hash_table_rehashing(hash_table_t *ht)
{
    char *tmp = (char *)ht->array->data;
    for(int i = 0; i < ht->table_size; i++)
    {
        list_clear(*(list_t **)tmp);
        tmp += ht->array->elem_size;
    }
    ht->table_size = ht->array->alloc_size *= ht->factor;
    ht->array->data = ht->array->iallocator->reallocate(NULL, 
                                                        ht->array->data,
                                                        ht->array->alloc_size * 
                                                        ht->array->elem_size);
    for(int i = ht->array->logic_size; i < ht->table_size; i++)
    {
        list_t *l = list_create(ht->copy_fn,
                                ht->dealloc_fn,
                                ht->compare_fn,
                                NULL);
        array_push_back(ht->array, &l);
    }
}

void
hash_table_print(hash_table_t *ht, 
                 void (*print_fn)(void *o))
{
    char *tmp = (char *)ht->array->data;
    for(int i = 0; i < ht->table_size; i++)
    {
        printf("slot[%d]: ", i); list_prt_shead(*(list_t **)tmp, print_fn);
        tmp += ht->array->elem_size;
    }
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
