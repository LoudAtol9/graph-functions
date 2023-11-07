#include "matriz_adjacencias.h"


struct adjacency_matrix* create_adjacency_matrix(int order, bool directed)
{
    int i = 0;
    struct adjacency_matrix* g = NULL;

    g = (struct adjacency_matrix*) malloc(sizeof(struct adjacency_matrix));

    g->size = 0;
    g->order = order;
    g->directed = directed;
    g->matrix = (int**) malloc(order * sizeof(int*));

    for (i = 0; i < order; i++)
        g->matrix[i] = (int*) malloc(order * sizeof(int));
    
    return g;
}


void free_adjacency_matrix(struct adjacency_matrix** g)
{
    int i;

    for (i = 0; i < (*g)->order; i++)
        free((*g)->matrix[i]);
    
    free((*g)->matrix);
    free((*g));

    *g = NULL;
}

struct adjacency_matrix* list_to_adjacency_matrix(struct adjacency_list* gl)
{
    int i;

    struct edge* g;
    struct cel* aux;
    struct adjacency_matrix* gm;

    gm = create_adjacency_matrix(gl->order, gl->directed);
    gm->size = gl->size;

    for (i = 0; i < gm->order; i++)
    {
        for (aux = gl->edge_list[i]; aux != NULL; aux = aux->next)
        {
            g = (struct edge*) aux->info;

            set_edge_matrix(gm, i, g->vertice, g->weight);
        }
    }
    
    return gm;
}


struct adjacency_matrix* file_to_adjacency_matrix(char* filename)
{
    int i = 0;
    int j = 0;

    int vert_a = 0;
    int vert_b = 0;
    int weight = 1;

    int order = 0;
    int size = 0;
    bool dirigido = false;
    int valorado = 0;

    FILE* fp = NULL;
    struct adjacency_matrix* g;

    fp = fopen(filename, "r+");

    fread(&order, sizeof(int), 1, fp);
    fread(&size, sizeof(int), 1, fp);
    fread(&i, sizeof(int), 1, fp);
    dirigido = (i == 1);
    fread(&valorado, sizeof(int), 1, fp);

    g = create_adjacency_matrix(order, dirigido);

    for (i = 0; i < order; i++)
    {
        fread(&vert_a, sizeof(int), 1, fp);
        fread(&vert_b, sizeof(int), 1, fp);

        if (valorado == 1)
            fread(&weight, sizeof(int), 1, fp);
        
        set_edge_matrix(g, vert_a, vert_b, weight);
    }

    fclose(fp);
    return g;
}

bool adjacency_matrix_is_weighted(struct adjacency_matrix* g)
{
    int i;
    int j;

    for (i = 0; i < g->order; i++)
    {
        for (j = 0; j < g->order; j++)
        {
            if (g->matrix[i][j] > 1)
                return true; 
        }
    }

    return false;
}

void adjacency_matrix_to_file(struct adjacency_matrix* g, char* filename)
{
    int i;
    int j;
    bool weighted;

    FILE* fp;

    weighted = adjacency_matrix_is_weighted(g);
    fp = fopen(filename, "w");

    fwrite(&(g->order), sizeof(int), 1, fp);
    fwrite(&(g->size), sizeof(int), 1, fp);
    fwrite(&(g->directed), sizeof(bool), 1, fp);
    fwrite(&weighted, sizeof(bool), 1, fp);

    for (i = 0; i < g->order; i++)
    {
        for (j = 0; j < g->order; j++)
        {
            if (g->matrix[i][j] != 0)
            {
                fwrite(&(i), sizeof(int), 1, fp);
                fwrite(&(j), sizeof(int), 1, fp);

                if (weighted)
                    fwrite(&(g->matrix[i][j]), sizeof(int), 1, fp);
            }  
        }
    }

    fflush(fp);
    fclose(fp);
}



/*
 * A --> B
 */
void set_edge_matrix(struct adjacency_matrix* g, int vert_A, int vert_B, int weight)
{
    if (g->matrix[vert_A][vert_B] == 0)
        g->size = g->size + 1;

    g->matrix[vert_A][vert_B] = weight;

    if (!(g->directed))
        g->matrix[vert_B][vert_A] = weight; 
}


int degree_of_vertice_matrix(struct adjacency_matrix* g, int vert)
{
    int i = 0;
    int degree = 0;

    for (i = 0; i < g->order; i++)
        degree = g->matrix[vert] != 0 ? degree + 1 : degree;
    
    return degree;
}


int classify_graph_matrix(struct adjacency_matrix* g)
{
    int i;
    int j;

    /*tem laco?*/

    for (i = 0; i < g->order; i++)
    {
        if (g->matrix[i][i] > 0)
            return PSEUDO_GRAPH; 
    }

    /*tem conexões paralelas?*/
    for (i = 0; i < g->order; i++)
    {
        for (j = 0; j < g->order; j++)
        {
            if (i != j && g->matrix[i][j] > 0 && g->matrix[j][i] > 0)
                return MULTI_GRAPH;
        }
    }

    return SIMPLE_GRAPH;
}


void aux_depth_first_search(struct adjacency_matrix* g, int* visited, int current)
{
    int i;

    visited[current] = 1;

    printf(" %d ", current);

    for (i = 0; i < g->order; i++)
    {
        if ((visited[i] == 0) && (g->matrix[current][i] == 1))
            aux_depth_first_search(g, visited, i);
    }
}

void depth_first_search_matrix(struct adjacency_matrix* g, int begin)
{
    int i;
    int* visited = NULL;

    visited = (int*) malloc(g->order * sizeof(int));

    for (i = 0; i < g->order; i++)
        visited[i] = 0;
        
    printf("A seguir veremos a Busca por Profundidade (comecando do grafo %d) :\n", begin);
    
    aux_depth_first_search(g, visited, begin);

    free(visited);
    putchar('\n');
}


void breadth_first_search_matrix(struct adjacency_matrix* g, int begin)
{
    int i = 0;

    int* vert = NULL;
    int* current = NULL;
    int* visited = NULL;

    struct queue* in_line = NULL;

    /*inserindo o comeco na fila*/
    vert = (int*) malloc(sizeof(int));
    *vert = begin;
    enqueue(&in_line, vert);

    /*alocando espaco pra lista e zerando-a*/
    visited = (int*) malloc(g->order * sizeof(int));
    for (i = 0; i < g->order; i++)
        visited[i] = 0;
    visited[begin] = 1;

    printf("A seguir veremos a Busca por Largura (comecando do grafo %d) :\n", begin);

    /*enquano tiver algo na lista*/
    while (in_line != NULL)
    {
        current = dequeue(&in_line);

        printf(" %d ", *current);

        for (i = 0; i < g->order; i++)
        {
            if ((visited[i] == 0) && (g->matrix[*current][i] == 1))
            {
                vert = (int*) malloc(sizeof(int));
                *vert = i;
                enqueue(&in_line, vert);

                visited[i] = 1;
            }
        }

        free(current);
    }
    putchar('\n');
    free(visited);
}


struct adjacency_matrix* min_tree_prim_matrix(struct adjacency_matrix* g, int begin)
{
    int i = 0;
    int j = 0;
    int new_vert = begin;
    int min_pos = 0;
    int count = 0;

    int* min = NULL;
    int* edge = NULL;
    int* visited = NULL;
    int* reached = NULL;

    struct cel* edge_list = NULL;
    struct cel* aux = NULL;
    struct cel* elem = NULL;

    struct adjacency_matrix* min_tree = NULL;

    visited = (int*) malloc(g->order * sizeof(int));
    for (i = 0; i < g->order; i++)
        visited[i] = 0;
    reached = (int*) malloc(g->order * sizeof(int));
    for (i = 0; i < g->order; i++)
        reached[i] = 0;

    min_tree = create_adjacency_matrix(g->order, g->directed);

    /*Enquanto nao tiver visitados todos os vertices (possiveis)*/
    do
    {
        min = NULL;
        visited[new_vert] = 1;
        reached[new_vert] = 1;

        /*salva os caminhos do vertice atual*/
        /*nao salva os caminhos pra vertices ja acessados*/
        for (i = 0; i < g->order; i++)
        {
            if (visited[i] == 0 && reached[i] == 0 && g->matrix[new_vert][i] != 0)
            {
                edge = (int*) malloc(sizeof(int) * 3);
                edge[0] = new_vert;
                edge[1] = i;
                edge[2] = g->matrix[new_vert][i];

                add_elem(&edge_list, edge);
            }
        }

         /*Acha o caminho de menor peso dos caminhos salvos*/
        count = 0;
        for (aux = edge_list; aux != NULL; aux = aux->next)
        {
            edge = (int*) aux->info;

            if (min == NULL)
            {
                min = edge;
                min_pos = count;
            }    
            else if ((min[2] > edge[2]) && (visited[edge[1]] == 0))
            {
                min = edge;
                min_pos = count;
            } 
            count++;         
        }

        //Já foi alcançado
        if (min != NULL)
            reached[min[1]] = 1; 

        //Tiro o vertice da lista
        return_elem(&edge_list, min_pos);

         /*Temos um novo vertice que vai ter os caminhos salvos*/
        if (min != NULL && visited[min[1]] == 0)
        {
            new_vert = min[1];
            set_edge_matrix(min_tree, min[0], min[1], min[2]);
            free(min);
        }

    } while (!is_array_true(visited, g->order) && edge_list != NULL);

    free_list(edge_list);
    free(visited);
    free(reached);

    return min_tree;
}


int dijkstra_matrix(struct adjacency_matrix* g, int orig, int dest)
{
    int i;
    int vert;
    int adj_vert;
    int edge_cost;
    int ret;

	int* dist = NULL;
    int* visited = NULL;
    int* pair = NULL;

    struct queue* q = NULL;

    dist = malloc(sizeof(int) * g->order);
    visited = malloc(sizeof(int) * g->order);	

	// inicia o vetor de distâncias e visitados
	for(int i = 0; i < g->order; i++)
	{
		dist[i] = INT_MAX;
		visited[i] = false;
	}

	// a distância de orig para orig é 0
	dist[orig] = 0;

	// insere na fila
    pair = malloc(sizeof(int) * 2);
    pair[0] = dist[orig];
    pair[1] = orig;
	enqueue(&q, pair);

	// Equanto tiver algo na fila
	while(q != NULL)
	{
		pair = dequeue(&q); 
		vert = pair[1]; 

		if(visited[vert] == false)
		{
			visited[vert] = true;

			// Acha os vértices adjacentes
			for(i = 0; i < g->order; i++)
			{
				// Caso exista:
                if (g->matrix[vert][i] != 0)
                {
                    adj_vert = i;
                    edge_cost = g->matrix[vert][i];

                    // Relaxamento
					if(dist[adj_vert] > (dist[vert] + edge_cost))
					{
						// Atualiza a distância de "adj_vert" e insere na fila
						dist[adj_vert] = dist[vert] + edge_cost;
                        pair = (int*) malloc(sizeof(int) * 2);
                        pair[0] = dist[adj_vert];
                        pair[1] = adj_vert;
                        enqueue(&q, pair);
					}
                }					
			}
		}
	}

	// Retorna a distância/custo
    ret = dist[dest];

    free(dist);
    free(visited);

	return ret;
}

int main()
{
    struct adjacency_matrix* g = create_adjacency_matrix(8, true);
    struct adjacency_matrix* min_tree = NULL;

    set_edge_matrix(g, 1, 2, 1);
    set_edge_matrix(g, 1, 3, 1);
    set_edge_matrix(g, 2, 4, 1);
    set_edge_matrix(g, 2, 5, 1);
    set_edge_matrix(g, 3, 6, 1);
    set_edge_matrix(g, 3, 7, 1);
    set_edge_matrix(g, 3, 6, 1);
    set_edge_matrix(g, 3, 4, 1);
    set_edge_matrix(g, 3, 3, 1);
    set_edge_matrix(g, 3, 5, 1);

    breadth_first_search_matrix(g, 1);
    depth_first_search_matrix(g, 1);

    printf("Grau do Vertice 1: %d\n", degree_of_vertice_matrix(g, 1));

    min_tree = min_tree_prim_matrix(g, 1);

    adjacency_list_to_file(g, "adjmatrix.graph");
    adjacency_list_to_file(min_tree, "adjmatrix_min-tree.graph");

    printf("custo %d\n", dijkstra_matrix(g, 1, 6));
    
    free(g);
    free(min_tree);
    exit(0);
}