#include "linked_list.h"

void add_elem(struct cel** head, void* info)
{
    struct cel** aux;
    struct cel* new;

    new = (struct cel*) malloc(sizeof(struct cel));
    new->info = info;
    new->next = NULL;

    for (aux = head; (*aux) != NULL; aux = &((*aux)->next));

    *aux = new;
}

void add_cel(struct cel** head, struct cel* elem)
{
    struct cel** aux;
    for (aux = head; (*aux) != NULL; aux = &((*aux)->next));
    *aux = elem;
}

void free_list(struct cel* ptr)
{
    if (ptr != NULL)
    {
        free_list(ptr->next);
        free(ptr->info);
        free(ptr);
    }
}

bool remove_elem(struct cel** head, int index)
{
    int i = 0;
    struct cel** aux;
    struct cel* delete;

    for (aux = head; (*aux) != NULL && i < index; aux = &((*aux)->next))
        i++;
    
    if ((*aux) == NULL)
        return false;
    
    delete = *aux;
    *aux = (*aux)->next;

    free(delete->info);
    free(delete);

    return true;
}

void* return_elem(struct cel** head, int index)
{
    int i = 0;
    struct cel** aux;
    struct cel* ret;
    void* info;

    for (aux = head; (*aux) != NULL && i < index; aux = &((*aux)->next))
        i++;
    
    if ((*aux) == NULL)
        return NULL;
    
    ret = *aux;
    *aux = (*aux)->next;

    info = ret->info;
    free(ret);
    return info;
}

int len_lista(struct cel* head)
{
    int i = 0;
    struct cel* aux;

    for (aux = head; aux != NULL; aux = aux->next)
        i++;
    
    return i;
}


struct cel* search_for_ptr(struct cel* head, void* ptr)
{
    struct cel* aux;

    for (aux = head; aux != NULL; aux = aux->next)
    {
        if (aux->info == ptr)
            return aux;
    }

    return NULL;
}

struct cel* search_for_pos(struct cel* head, int pos)
{
    int i = 0;
    struct cel* aux;

    for (aux = head; aux != NULL; aux = aux->next)
    {
        if (i == pos)
            return aux;
        i++;
    }

    return NULL;    
}

