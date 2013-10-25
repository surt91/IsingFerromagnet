/*! \file */
#ifndef _BFS_H
#define _BFS_H

#include "graph.h"
#include "queue.h"
#include "stack.h"

#define SQUARE(x) (x)*(x)

enum colors {white, gray, black};

typedef struct
{
    int L;
    int number_of_nodes;
    int number_of_clusters;
    int *map;
    int *sizes;

} cluster_map_t;

void print_clustermap(cluster_map_t *cluster_map);
cluster_map_t *bfs_cluster(gs_graph_t *g);

#endif /* _BFS_H */
