#include "bfs.h"

/*! \fn void bfs_cluster(gs_graph_t *g, int *cluster_map)
    \brief Führt eine Breitensuche nach clustern durch

    Findet alle Cluster und liefert ein Array mit Clusterindizes an den
    Gitterplätzen

    \param [in]     g                zu durchsuchender Graph
    \param [in,out] cluster_map      Array mit Clusterindizes
*/
cluster_map_t *bfs_cluster(gs_graph_t *g)
{
    int n, u, cluster_count;
    short int seed_spin;
    enum colors *color;
    int seed;
    int size;
    int neighbor_index;
    queue_t *q;
    queue_t *tmp_cluster_size;
    cluster_map_t *cluster_map;

    color = (enum colors *) malloc(g->num_nodes*sizeof(enum colors));
    cluster_map = (cluster_map_t *) malloc(sizeof(cluster_map_t));
    cluster_map->map = (int *) malloc(g->num_nodes*sizeof(int));
    cluster_map->number_of_nodes = g->num_nodes;
    cluster_map->L = g->L;

    for(n=0; n<g->num_nodes; n++)
    {
        color[n] = white;
        cluster_map->map[n] = -1;
    }
    cluster_count = 0;
    tmp_cluster_size = create_queue();

    for(seed=0; seed<g->num_nodes; seed++)
    {
        if(color[seed] == white)
        {
            seed_spin = g->spins[seed];
            size = 0;
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
                size++;
                cluster_map->map[u] = cluster_count;
            }
            clear_queue(q);
            enqueue(tmp_cluster_size,size);
            cluster_count++;
        }
    }
    free(color);

    cluster_map->number_of_clusters = cluster_count-1;
    cluster_map->sizes = (int *) malloc((cluster_count-1)*sizeof(int));
    n = 0;
    while(tmp_cluster_size->size)
    {
        cluster_map->sizes[n] = dequeue(tmp_cluster_size);
        n++;
    }

    return cluster_map;
}

/*! \fn void print_clustermap(gs_graph_t *g, int *cluster_map)
    \brief Druckt eine "Cluster Karte" nach stdout

    \param [in]     g                Graph
    \param [in]     cluster_map      Array mit Clusterindizes
*/
void print_clustermap(cluster_map_t *cluster_map)
{
    int i,j,L;
    L = cluster_map->L;
    for(i=0;i<L;i++)
    {
        for(j=0;j<L;j++)
            printf("% 3d ",cluster_map->map[i*L+j]);
        printf("\n");
    }
    for(i=0;i<cluster_map->number_of_clusters;i++)
    {
        printf("% 3d: %d nodes\n",i,cluster_map->sizes[i]);
    }
}
