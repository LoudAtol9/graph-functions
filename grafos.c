#include "grafos.h"

struct graph* create_graph(int order, bool directed, enum implementation type)
{
    struct graph* g;

    g = (struct graph*) malloc(sizeof(struct graph));

    switch (g->type)
    {
    case adj_matrix:
        g->graph = create_adjacency_matrix(order, directed);
        break;
    
    case adj_list:
        g->graph = create_adjacency_list(order, directed);
        break;
    }

    return g;
}