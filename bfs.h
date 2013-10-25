/*! \file */
#ifndef _BFS_H
#define _BFS_H

#include "graph.h"
#include "queue.h"
#include "stack.h"

enum colors {white, gray, black};

void print_clustermap(gs_graph_t *g, int *cluster_map);
void bfs_cluster(gs_graph_t *g, int *cluster_map);

#endif /* _BFS_H */
