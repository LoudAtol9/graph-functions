#ifndef GRAFOS_H
#define GRAFOS_H

#include "matriz_adjacencias.h"
#include "lista_adjacencias.h"

enum implementation 
{
    adj_matrix = 0,
    adj_list = 1
};

struct graph
{
    enum implementation type;
    void* graph;
};


#endif