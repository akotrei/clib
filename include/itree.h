#ifndef __ITREE_H__
#define __ITREE_H__

typedef struct _itree itree;

typedef struct _itree 
{
    void* get_data(void* self);
    itree* get_left(void* self);
    itree* get_right(void* self);
    void* add_elem(void* self, void* elem);
    void* del_elem(void* self, void* elem);
    void* fnd_elem(void* self, void* elem);    
    void *self;
};

#endif
