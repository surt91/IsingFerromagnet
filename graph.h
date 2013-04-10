/*! \file */
#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*! \var gs_node_t;
    \brief Knoten in einem Graphen

    Dabei stellt dieser Knoten einen Spin dar, dessen Position durch x
    und y, dessen Status durch int spin und dessen Nachbarn in der Liste
    elem_t *neighbors als Indizes gegeben sind.
*/
typedef struct
{
    double x,y;                                  //!< Position der Spins
    int spin;                                     //!< Up = 1; down = -1
    elem_t *neighbors;               //!< Pointer auf Liste der Nachbarn
} gs_node_t;

/*! \var gs_graph_t;
    \brief Ein Graph

    Er besteht aus der Anzahl seiner Knoten und einem Array, in dem Pointer
    auf seine Knoten liegen.
*/
typedef struct
{
    int         num_nodes;            //!< Anzahl der Knoten, Normal L^2
    int                 L;                              //!< Seitenlänge
    double          T,M,E;                  //!< Observablen des Systems
    gs_node_t       *node;               //!< Pointer auf ein Array, das die Knoten enthält
} gs_graph_t;

void gs_insert_edge(gs_graph_t *g, int from, int to, double weight);
gs_graph_t *gs_create_graph(int L);
void print_graph_for_graph_viz(gs_graph_t *g);
void print_graph_for_gnuplot(gs_graph_t *g);
void gs_clear_graph(gs_graph_t *g);
int gs_edge_exists(gs_graph_t *g, int from, int to);

#endif /* _GRAPH_H */
