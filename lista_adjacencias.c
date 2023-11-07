#include "lista_adjacencias.h"


struct adjacency_list* create_adjacency_list(int order, bool directed)
{
    struct adjacency_list* g = NULL;

    g = (struct adjacency_list*) malloc(sizeof(struct adjacency_list));

    g->edge_list = (struct cel**) malloc(order * sizeof(struct cel*));
    g->order = order;
    g->directed = directed;
    g->size = 0;
    
    return g;
}

void free_adjacency_list(struct adjacency_list** g)
{
    int i;

    for (i = 0; i < (*g)->order; i++)
        free_list((*g)->edge_list[i]);
    
    free((*g)->edge_list);
    free(*g);
    *g = NULL;
}

struct adjacency_list* matrix_to_adjacency_list(struct adjacency_matrix* gm)
{
    int i = 0;
    int j = 0;
    struct adjacency_list* gl;

    gl = create_adjacency_list(gm->order, gm->directed);
    gl->size = gm->size;

    for (i = 0; i < gl->order; i++)
    {
        for (j = 0; j < gl->order; j++)
        {
            if (gm->matrix[i][j] != 0)
                set_edge_list(gl, i, j, gm->matrix[i][j]);
        }
        
    }

    return gl;
}


struct adjacency_list* file_to_adjacency_list(char* filename)
{
    int i = 0;

    int vert_a = 0;
    int vert_b = 0;
    int weight = 1;

    int order = 0;
    int size = 0;
    bool directed = false;
    bool weighted = false;

    FILE* fp = NULL;
    struct adjacency_list* g;

    fp = fopen(filename, "r+");

    fread(&order, sizeof(int), 1, fp);
    fread(&size, sizeof(int), 1, fp);
    fread(&directed, sizeof(bool), 1, fp);
    fread(&weighted, sizeof(bool), 1, fp);

    g = create_adjacency_list(order, directed);

    for (i = 0; i < size; i++)
    {
        fread(&vert_a, sizeof(int), 1, fp);
        fread(&vert_b, sizeof(int), 1, fp);

        if (weighted)
            fread(&weight, sizeof(int), 1, fp);
        
        set_edge_list(g, vert_a, vert_b, weight);
    }

    fclose(fp);

    return g;
}


void adjacency_list_to_file(struct adjacency_list* g, char* filename)
{
    int i;
    bool weighted;

    struct cel* aux;
    struct edge* g1;

    FILE* fp;

    weighted = adjacency_list_is_weighted(g);
    fp = fopen(filename, "w+");

    fwrite(&(g->order), sizeof(int), 1, fp);
    fwrite(&(g->size), sizeof(int), 1, fp);
    fwrite(&(g->directed), sizeof(bool), 1, fp);
    fwrite(&weighted, sizeof(bool), 1, fp);

    for (i = 0; i < g->order; i++)
    {
        for (aux = g->edge_list[i]; aux != NULL; aux = aux->next)
        {
            g1 = ((struct edge*) aux->info);

            fwrite(&(i), sizeof(int), 1, fp);
            fwrite(&(g1->vertice), sizeof(int), 1, fp);

            if (weighted)
                fwrite(&(g1->weight), sizeof(int), 1, fp);  
        }
    }

    fflush(fp);
    fclose(fp);
}

bool adjacency_list_is_weighted(struct adjacency_list* g)
{
    int i;
    struct cel* aux;
    struct edge* g1;

    for (i = 0; i < g->order; i++)
    {
        for (aux = g->edge_list[i]; aux != NULL; aux = aux->next)
        {
            g1 = (struct edge*) aux->info;

            if (g1->weight != 1)
                return true; 
        }
    }

    return false;
}

void set_edge_list(struct adjacency_list* g, int vert_A, int vert_B, int weight)
{
    bool pre_existing_edge = false;

    struct edge* g1;
    struct edge* g2;
    struct edge* g_aux;

    struct cel* ptr;

    /*Já tem o vertice na lista?*/
    for (ptr = g->edge_list[vert_A]; ptr != NULL; ptr = ptr->next)
    {
        g_aux = (struct edge*) ptr->info;

        if (g_aux->vertice == vert_B)
        {
            g_aux->weight = weight;
            pre_existing_edge = true;
        }
    }
    

    /*Se tiver e nao for dirigido tem que ter a paridade*/
    if (!(g->directed) && pre_existing_edge)
    {
        for (ptr = g->edge_list[vert_B]; ptr != NULL; ptr = ptr->next)
        {
            g_aux = (struct edge*) ptr->info;

            if (g_aux->vertice == vert_A)
            {
                g_aux->weight = weight;
                pre_existing_edge = true;
            }
        }
    }
    
    /*Se nao existir crie*/
    if (!pre_existing_edge)
    {
        g1 = (struct edge*) malloc(sizeof(struct edge));
        g1->vertice = vert_B;
        g1->weight = weight;
        add_elem(&(g->edge_list[vert_A]), g1);

        g->size = g->size + 1;

        /*Se nao for dirigido tem que ter paridade*/
        if (!(g->directed))
        {
            g2 = (struct edge*) malloc(sizeof(struct edge));
            g2->vertice = vert_B;
            g2->weight = weight;
            add_elem(&(g->edge_list[vert_B]), g2);
        }
    }
}



int degree_of_vertice_list(struct adjacency_list* g, int vert)
{
    struct cel* aux;
    aux = g->edge_list[vert];

    return len_lista(aux);
}

void aux_depth_first_search(struct adjacency_list* g, int* visited, int current)
{
    struct cel* aux;
    struct edge* g1;

    visited[current] = 1;

    printf(" %d ", current);

    for (aux = g->edge_list[current]; aux != NULL; aux = aux->next)
    {
        g1 = (struct edge*) aux->info;

        if (visited[g1->vertice] == 0)
            aux_depth_first_search(g, visited, g1->vertice);
    }
}

void depth_first_search_list(struct adjacency_list* g, int begin)
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


void breadth_first_search_list(struct adjacency_list* g, int begin)
{
    int i;

    int* vert = NULL;
    int* current = NULL;
    int* visited = NULL;

    struct cel* aux = NULL;
    struct edge* g1 = NULL;
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

    /*enquanto tiver algo na lista*/
    while (in_line != NULL)
    {
        current = (int*) in_line->info;
        dequeue(&in_line);

        printf(" %d ", *current);

        for (aux = g->edge_list[*current]; aux != NULL; aux = aux->next)
        {
            g1 = (struct edge*) aux->info;

            if (visited[g1->vertice] == 0)
            {
                vert = (int*) malloc(sizeof(int));
                *vert = g1->vertice;
                enqueue(&in_line, vert);

                visited[g1->vertice] = 1;
            }
        }

        free(current);
    }

    free(visited);
    putchar('\n');
}


void prufer_code_list(struct adjacency_list* g)
{}


struct adjacency_list* min_tree_prim_list(struct adjacency_list* g, int begin)
{
    int i = 0;
    int cont = 0;
    int min_pos = 0;
    int new_vert = begin;
    int* visited = NULL;
    int* reached = NULL;

    struct edge_data* e = NULL;
    struct edge_data* min = NULL;

    struct cel* edge_list = NULL;
    struct cel* aux = NULL;
    struct cel* elem = NULL;

    struct adjacency_list* min_tree = NULL;

    visited = (int*) malloc(g->order * sizeof(int));
    for (i = 0; i < g->order; i++)
        visited[i] = 0;
    reached = (int*) malloc(g->order * sizeof(int));
    for (i = 0; i < g->order; i++)
        reached[i] = 0;

    min_tree = create_adjacency_list(g->order, g->directed);

    /*Enquanto nao tiver visitados todos os vertices*/
    
    do
    {
        min = NULL;
        visited[new_vert] = 1;
        reached[new_vert] = 1;

        /*salva os caminhos do vertice atual*/
        /*nao salva os caminhos pra vertices ja acessados*/
        for (aux = g->edge_list[new_vert]; aux != NULL; aux = aux->next)
        {
            if (visited[((struct edge*) aux->info)->vertice] == 0 && reached[((struct edge*) aux->info)->vertice] == 0)
            {
                e = (struct edge_data*) malloc(sizeof(struct edge_data));
                e->edge = (struct edge*) aux->info;
                e->origin = new_vert;

                elem = (struct cel*) malloc(sizeof(struct cel));
                elem->info = e;
                elem->next = NULL;

                add_cel(&edge_list, elem);
            } 
        }

        /*Acha o caminho de menor peso dos caminhos salvos*/
        cont = 0;    
        for (aux = edge_list; aux != NULL; aux = aux->next)
        {
            e = (struct edge_data*) aux->info;

            if (min == NULL)
            {
                min = e;
                min_pos = cont;
            }
            else if ((min->edge->weight > e->edge->weight) && (visited[e->edge->vertice] == 0))
            {
                min = e; 
                min_pos = cont;
            }   

            cont++;           
        }

        //Já foi alcançado
        reached[min->edge->vertice] = 1; 

        //Tiro o vertice da lista
        return_elem(&edge_list, min_pos);

        /*Temos um novo vertice que vai ter os caminhos salvos*/
        if (min != NULL && visited[min->edge->vertice] == 0)
        {
            new_vert = min->edge->vertice;
            set_edge_list(min_tree, min->origin, new_vert, min->edge->weight);
        }

    } while (!is_array_true(visited, g->order) && edge_list != NULL);

    free_list(edge_list);
    free(visited);
    free(reached);

    return min_tree;
}


int dijkstra_list(struct adjacency_list* g, int orig, int dest)
{
    int i = 0;
    int vert;
    int adj_vert;
    int edge_cost;
    int ret;

	int* dist = NULL;
    int* visited = NULL;
    int* pair = NULL;

    struct cel* aux = NULL;
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
			for(aux = g->edge_list[vert]; aux != NULL; aux = aux->next)
			{
                adj_vert = ((struct edge*) aux->info)->vertice;
                edge_cost = ((struct edge*) aux->info)->weight;

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

	// Retorna a distância/custo
    ret = dist[dest];

    free(dist);
    free(visited);

	return ret;
}

int main()
{
    struct adjacency_list* g = create_adjacency_list(8, true);
    struct adjacency_list* min_tree;

    set_edge_list(g, 1, 2, 1);
    set_edge_list(g, 1, 3, 1);
    set_edge_list(g, 2, 4, 1);
    set_edge_list(g, 2, 5, 1);
    set_edge_list(g, 3, 6, 1);
    set_edge_list(g, 3, 7, 1);
    set_edge_list(g, 3, 6, 1);
    set_edge_list(g, 3, 4, 1);
    set_edge_list(g, 3, 3, 1);
    set_edge_list(g, 3, 5, 1);

    breadth_first_search_list(g, 1);
    depth_first_search_list(g, 1);

    printf("Grau do Vertice 1: %d\n", degree_of_vertice_list(g, 1));

    min_tree = min_tree_prim_list(g, 1);

    adjacency_list_to_file(g, "adjlist.graph");
    adjacency_list_to_file(min_tree, "adjlist_min-tree.graph");
    
    printf("custo %d\n", dijkstra_list(g, 1, 6));

    free(g);
    free(min_tree);
    exit(0);
}


