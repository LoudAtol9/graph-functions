#ifndef MATRIZ_ADJACENCIAS_H
#define MATRIZ_ADJACENCIAS_H

#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "utils/queue.h"

#include "lista_adjacencias.h"

#define SIMPLE_GRAPH 0
#define MULTI_GRAPH 1
#define PSEUDO_GRAPH 2 

struct adjacency_matrix
{
    bool directed;

    int order;
    int size;
    int** matrix;
};

struct adjacency_matrix* create_adjacency_matrix(int order, bool directed);

void free_adjacency_matrix(struct adjacency_matrix** g);

void set_edge_matrix(struct adjacency_matrix* g, int vert_A, int vert_B, int weight);

int degree_of_vertice_matrix(struct adjacency_matrix* g, int vert);

int classify_graph_matrix(struct adjacency_matrix* g);

void depth_first_search_matrix(struct adjacency_matrix* g, int begin);

void breadth_first_search_matrix(struct adjacency_matrix* g, int begin);

#endif