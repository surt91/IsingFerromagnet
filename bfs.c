#include "bfs.h"

/*! \fn void bfs_cluster(gs_graph_t *g, int *cluster_map)
    \brief Führt eine Breitensuche nach clustern durch

    Findet alle Cluster und liefert ein Array mit Clusterindizes an den
    Gitterplätzen

    \param [in]     g                zu durchsuchender Graph
    \param [in,out] cluster_map      Array mit Clusterindizes
*/
void bfs_cluster(gs_graph_t *g, int *cluster_map)
{
    int n, u, cluster_count;
    short int seed_spin;
    enum colors *color;
    int seed;
    int neighbor_index;
    queue_t *q;

    color = (enum colors *) malloc(g->num_nodes*sizeof(enum colors));
    for(n=0; n<g->num_nodes; n++)
    {
        color[n] = white;
        cluster_map[n] = -1;
    }
    cluster_count = 0;

    for(seed=0; seed<g->num_nodes; seed++)
    {
        if(color[seed] == white)
        {
            cluster_count++;
            seed_spin = g->spins[seed];
            q = create_queue();

            color[seed] = gray;
            enqueue(q,seed);

            while(q->size)
            {
                u = dequeue(q);

                for(n=0;n<g->node[u].num_neighbors;n++)
                {
                    neighbor_index = g->node[u].neighbors[n].index;
                    if(color[neighbor_index] == white && g->spins[neighbor_index] == seed_spin)
                    {
                        color[neighbor_index] = gray;
                        enqueue(q,neighbor_index);
                    }
                }
                color[u] = black;
                cluster_map[u] = cluster_count;
            }
            clear_queue(q);
        }
    }
    free(color);
}

/*! \fn void print_clustermap(gs_graph_t *g, int *cluster_map)
    \brief Druckt eine "Cluster Karte" nach stdout

    \param [in]     g                Graph
    \param [in]     cluster_map      Array mit Clusterindizes
*/
void print_clustermap(gs_graph_t *g, int *cluster_map)
{
    int i,j;
    for(i=0;i<g->L;i++)
    {
        for(j=0;j<g->L;j++)
            printf("% 3d ",cluster_map[i*g->L+j]);
        printf("\n");
    }
}
