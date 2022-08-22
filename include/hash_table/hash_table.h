#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include "hash_table_type_public.h"
#include <stdint.h>
#include <stddef.h>
#include "interface.h"

/* 
 */
hash_table_t* 
hash_table_create(int table_size,
                  uint64_t (*hash_fn)(const void *),
                  int (*compare_fn)(void *, void *),
                  void* (*copy_fn)(void *),
                  void (*dealloc_fn)(void *),
                  int factor,
                  iallocator_t *iallocator);

/*
 */
void 
hash_table_delete(hash_table_t *ht);

/*
 */
void 
hash_table_insert(hash_table_t *ht, 
                  void *key, 
                  void *value);

/*
 */
void*
hash_table_search(hash_table_t *ht, 
                  void *key);

/*
 */
void*
hash_table_remove(hash_table_t *ht, 
                  void *key);

/*
 */
void 
hash_table_factor(hash_table_t *ht, 
                  int factor);

/*
 */
void 
hash_table_rehashing(hash_table_t *ht);

/*
 */
void
hash_table_print(hash_table_t *ht, 
                 void (*print_fn)(void *o));

#endif
