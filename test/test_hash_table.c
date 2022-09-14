#include "hash_table.h"
#include "hash_table_type_private.h"
#include "allocator_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define HASH_TABLE_SIZE 10

typedef struct object
{
    int x;
    int y;
} object;

typedef struct pair
{
    char *key;
    char *value;
} pair;

char *rand_string(size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    char *str = (char *)malloc(size * sizeof(char));
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

void test_hash_table_1();
uint64_t test_hash_table_hash_function(void *o)
{
    const char *s = (const char *)o;
    const int p = 31, m = 1e9 + 7;
    int hash = 0;
    long p_pow = 1;
    for(int i = 0; i < strlen(s); i++) {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash;
//    int *key = (int *)o;
//    return *key;
}

void test_hash_table_print(void *o)
{
    pair *a = (pair *)o;
    printf("[key: %s, value: %s]", a->key, a->value);
}

void test_array_dealloc_fn(void *o)
{
	int *a = (int *)o;
    free(a);
}

int cmp(void *o1, void *o2)
{
    char *o_1 = (char *)o1;
    char *o_2 = (char *)o2;
    if(strcmp(o_1, o_2) == 0)
        return 0;
    return -1;
}

int main(int argc, char **args)
{
    test_hash_table_1();
    return 0;
}

void test_hash_table_1()
{
    char mem[sizeof(iallocator_t)];
    iallocator_t *iallocator = allocator_std_new(mem);
    printf("%d\n", hash_table_sizeof());
    hash_table_t *ht = hash_table_create(iallocator->allocate(NULL, hash_table_sizeof()),
                                         HASH_TABLE_SIZE,
                                         test_hash_table_hash_function,
                                         cmp,
                                         NULL, NULL, NULL, NULL,
                                         2, iallocator);

    hash_table_delete(ht);

/*    hash_table_t *ht = hash_table_create(HASH_TABLE_SIZE,
                                         test_hash_table_hash_function,
                                         cmp,
                                         NULL,
                                         NULL,
                                         2,
                                         NULL);
    pair p[HASH_TABLE_SIZE];

    int random;
    for(int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        random = rand() % 25 - 5;
        if(random < 0)
            random = -1 * random;
        else if (random == 0)
            random += 10;
        p[i].key = rand_string(random);

        random = rand() % 25 - 5;
        if(random < 0)
            random = -1 * random;
        else if (random == 0)
            random += 10;
        p[i].value = rand_string(random);

        hash_table_insert(ht, p[i].key, p[i].value);
    }

    hash_table_print(ht, test_hash_table_print);
    pair *f = (pair *)hash_table_search(ht, p[5].key);
    pair *c = (pair *)hash_table_remove(ht, p[5].key);

    hash_table_rehashing(ht);

    hash_table_print(ht, test_hash_table_print);
    printf("%s, %s\n", c->key, c->value);
    printf("%s, %s\n", f->key, f->value);
    printf("%d\n", j);
    hash_table_delete(ht);*/
}
