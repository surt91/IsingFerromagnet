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

    cluster_map->number_of_clusters = cluster_count;
    cluster_map->sizes = (int *) malloc((cluster_count)*sizeof(int));
    n = 0;
    while(tmp_cluster_size->size)
    {
        cluster_map->sizes[n] = dequeue(tmp_cluster_size);
        n++;
    }

    return cluster_map;
}

/*! \fn double calc_radius_of_gyration(gs_graph_t *g, int *cluster_map, int cluster_label)
    \brief Berechnet das Quadrat vom radius of gyration des Clusters cluster_label

    \f$ R_s^2 = \frac {1}{s} \sum_{i=1}^s (r_i-<r>)^2 \f$

    \param [in]     g                Graph
    \param [in]     cluster_map      Array mit Clusterindizes
    \param [in]     cluster_label    label des zu untersuchenden clusters
    \return Radius of gyration squared
*/
double calc_radius_of_gyration(gs_graph_t *g, cluster_map_t *cluster_map, int cluster_label)
{
    int n;
    int rx_mean, ry_mean;
    int R;

    // mittelpunkt
    rx_mean = 0; ry_mean = 0;
    for(n=0; n<g->num_nodes; n++)
        if(cluster_map->map[n] == cluster_label)
        {
            rx_mean += g->node[n].x;
            ry_mean += g->node[n].y;
        }
    rx_mean /= cluster_map->sizes[cluster_label];
    ry_mean /= cluster_map->sizes[cluster_label];

    // radius of gyration
    R = 0;
    for(n=0; n<g->num_nodes; n++)
        if(cluster_map->map[n] == cluster_label)
        {
            R += SQUARE(g->node[n].x - rx_mean) + SQUARE(g->node[n].y - ry_mean);
        }
    R /= cluster_map->sizes[cluster_label];

    return R;
}

int get_label_of_biggest_cluster(cluster_map_t *cluster_map)
{
    int i;
    int max = 0;
    int max_index = 0;

    for(i=0;i<cluster_map->number_of_clusters;i++)
    {
        if(cluster_map->sizes[i] > max)
        {
            max_index = i;
            max = cluster_map->sizes[i];
        }
    }

    return max_index;
}

/*! \fn double calc_connectedness_length(gs_graph_t *g, cluster_map_t *cluster_map)
    \brief Berechnet die connectedness length

    \f$ \xi^2 = \frac {\sum_s s^2 n_s <R_s^2>}{\sum_s s^2 n_s} \f$

    \param [in]     g                Graph
    \param [in]     cluster_map      Array mit Clusterindizes
    \return connecedness length
*/
double calc_connectedness_length(gs_graph_t *g, cluster_map_t *cluster_map)
{
    int i, j;
    int s, ns;
    double Rs;
    double zahler, nenner;
    int *visited;

    if(cluster_map->number_of_clusters == 1)
        return 0.;

    visited = (int *) calloc(cluster_map->number_of_clusters, sizeof(int));

    zahler = 0; nenner = 0;
    visited[get_label_of_biggest_cluster(cluster_map)] = 1;

    for(i=0;i<cluster_map->number_of_clusters;i++)
        if(!visited[i])
        {
            s = cluster_map->sizes[i];
            ns = 0;
            Rs = 0;
            for(j=0;j<cluster_map->number_of_clusters;j++)
                if(cluster_map->sizes[j] == s)
                {
                    visited[j] = 1;
                    ns++;
                    Rs += calc_radius_of_gyration(g, cluster_map, j);
                }
            zahler += s*s*Rs;
            nenner += s*s*ns;
        }
    return sqrt(zahler/nenner);
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
