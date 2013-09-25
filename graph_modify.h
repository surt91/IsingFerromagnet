/*! \file graph_modify.h */

#ifndef _GRAPH_MODIFIY_H
#define _GRAPH_MODIFIY_H

#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_rng.h>

#include "graph.h"
#include "spin.h"
#include "list.h"
#include "tree.h"

/*! \def MIN(X,Y)
 * Minimum von zwei Zahlen
 */
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
/*! \def MAX(X,Y)
 * Maximum von zwei Zahlen
 */
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

gs_graph_t **init_graphs(const options_t options);

void move_graph_nodes(gs_graph_t *g, double (*f)(const gsl_rng *, double), gsl_rng *rng, const double sigma);

void create_edges(gs_graph_t *g, const options_t o);
inline int check_relative_neighborhood(double dist12, gs_node_t node1, gs_node_t node2, gs_node_t node3);
inline void get_cell_border_relative_neighborhood(gs_node_t node1, gs_node_t node2, double dist12, int L, int *x0, int *x1, int *y0, int *y1);
inline int check_gabriel(double dist12, gs_node_t node1, gs_node_t node2, gs_node_t node3);
inline void get_cell_border_gabriel(gs_node_t node1, gs_node_t node2, double dist12, int L, int *x0, int *x1, int *y0, int *y1);
void delete_random_edges_till_percolation(gs_graph_t *g, gsl_rng *rng);
int test_connectivity_between_nodes(gs_graph_t *g, int node1, int node2);
node_t * gs_depth_first_search(gs_graph_t *g, int node, int target, node_t *tree_of_tested_nodes, int *status);
double get_mean_dist(gs_graph_t *g);
double get_mean_weight(gs_graph_t *g);
double get_mean_deg(gs_graph_t *g);
double get_sum_weight(gs_graph_t *g);

void init_spins_randomly(gs_graph_t *g, gsl_rng *rng);
void init_spins_up(gs_graph_t *g);

#endif /*GRAPH_MODIFIY_H*/
