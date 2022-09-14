#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include "hash_table_type_public.h"
#include <stdint.h>
#include <stddef.h>
#include "interface.h"

/*
 * DESCRIPTION
 * The work of the hash table is based on the fact that a hash function will
 * be used that will generate a hash code and these hash codes will play the
 * role of keys in the hash table and values ​​will be inserted into them. Since
 * for searching and deleting keys it is worth calculating only the hash code
 * and using it to refer to the value in the hash table, the operation of
 * deletion and search are constant, as well as additions. To resolve
 * collisions in the hash table (obtaining the same index from the hash code),
 * the chain method is used, that is, values ​​will be added to the singly linked
 * list at the same index.
 */

/*
 * @hash_table_create function that creates a hash table and returns a pointer
 * to it
 *
 * @buffer         - pointer to the memory area that will be used to initialize
 *                   the hash table
 *
 * @table_size     - hash table size
 *
 * @hash_fn        - pointer to the hash function that the user will pass, if
 *                   he passes NULL, then the standard hash function will be
 *                   used
 *
 * @compare_fn     - pointer to a key comparison function in a hash table
 *
 * @key_copy_fn    - pointer to copy key function, if NULL, pointer to key will
 *                   be returned
 *
 * @val_copy_fn    - pointer to value copy function, if NULL, pointer to value
 *                   will be returned
 *
 * @key_dealloc_fn - pointer to the key deletion function, if NULL then the
 *                   function that does nothing will be used
 *
 * @val_dealloc_fn - pointer to a value removal function, if NULL, a function
 *                   that does nothing will be used
 *
 * @factor         - parameter responsible for rehashing the table when it
 *                   is full
 *
 * @iallocator     - pointer to allocator interface
 */
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
                  iallocator_t *iallocator);

/*
 * @hash_table_delete function of deleting a hash table
 *
 * @ht - pointer to a hash table
 */
void
hash_table_delete(hash_table_t *ht);

/*
 * @hash_table_sizeof function returns size in the bytes of the structure
 * hash table
 */
int
hash_table_sizeof();

/*
 * @hash_table_insert function of insert value to a hash table
 *
 * @ht    - pointer to a hash table
 *
 * @key   - pointer to a key
 *
 * @value - pointer to a value
 */
void
hash_table_insert(hash_table_t *ht,
                  void *key,
                  void *value);

/*
* @hash_table_search function of searching in a hash table
*
* @ht  - pointer to a hash table
*
* @key - pointer to a key
*/
void*
hash_table_search(hash_table_t *ht,
                  void *key);

/*
* @hash_table_search function of removing value and key in a hash table
*
* @ht  - pointer to a hash table
*
* @key - pointer to a key
*/
void*
hash_table_remove(hash_table_t *ht,
                  void *key);

/*
 * @hash_table_rehashing function of rehashing a hash table
 *
 * @ht  - pointer to a hash table
 */
void
hash_table_rehashing(hash_table_t *ht);

/*
 * @hash_table_factor function to change the parameter that is responsible for
 * table rehashing
 *
 * @ht     - pointer to a hash table
 *
 * @factor - value to which the parameter @factor in a hash table will be
 *           changed
 */
void
hash_table_factor(hash_table_t *ht,
                  int factor);

#endif /* __HASH_TABLE_H__ */
