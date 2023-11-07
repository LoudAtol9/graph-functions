#ifndef LISTA_ADJACENCIAS_H
#define LISTA_ADJACENCIAS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "utils/linked_list.h"
#include "utils/stack.h"
#include "utils/queue.h"

#include "utils/arrays_utils.h"

#include "matriz_adjacencias.h"


#define SIMPLE_GRAPH 0
#define MULTI_GRAPH 1
#define PSEUDO_GRAPH 2 


struct edge
{
    int weight;
    int vertice;
};

struct edge_data 
{
    int origin;
    struct edge* edge;
};

struct adjacency_list
{
    bool directed;

    int order;
    int size;

    struct cel** edge_list;
};

struct adjacency_list* create_adjacency_list(int order, bool directed);

void free_adjacency_list(struct adjacency_list** g);

bool adjacency_list_is_weighted(struct adjacency_list* g);

void set_edge_list(struct adjacency_list* g, int vert_A, int vert_B, int weight);

int degree_of_vertice_list(struct adjacency_list* g, int vert);

void depth_first_search_list(struct adjacency_list* g, int begin);

void breadth_first_search_list(struct adjacency_list* g, int begin);

#endif