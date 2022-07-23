#ifndef __LIST_H__
#define __LIST_H__

#include "list_type_public.h"
#include "interface.h"

list_t* list_create(void* (*copy_fn)(void* obj),
                    void (*dealloc_fn)(void* obj),
                    void (*cmp_fn)(void* obj1, void* obj2),
                    iallocator_t* iallocator);
void list_delete(list_t* list);

void list_add_head(list_t* list, void* obj);
void list_add_tail(list_t* list, void* obj);
void* list_rmv_head(list_t* list);
void* list_rmv_tail(list_t* list);

void* list_rmv_shead(list_t* list, void* obj);
void* list_rmv_stail(list_t* list, void* obj);

void* list_fnd_shead(list_t* list, void* obj);
void* list_fnd_stail(list_t* list, void* obj);

void* list_get_head(list_t* list);
void* list_get_tail(list_t* list);
void* list_get_next(list_t* list);
void* list_get_prev(list_t* list);


#endif /* __LIST_H__ */