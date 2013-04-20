/*! \file */
#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>
#include <stdlib.h>

/*! \struct edge_t;
    \brief Kante in einem Graphen
*/
typedef struct
{
    int     index;               //!< Der Index des referenzierten Spins
    double  weight;                               //!< Gewicht der Kante
} gs_edge_t;

/*! \struct gs_node_t;
    \brief Knoten in einem Graphen

    Dabei stellt dieser Knoten einen Spin dar, dessen Position durch x
    und y, dessen Status durch int spin und dessen Nachbarn in der Liste
    elem_t *neighbors als Indizes gegeben sind.
*/
typedef struct
{
    int spin;                                     //!< Up = 1; down = -1
    double x;                        //!< horizontale Position des Spins
    double y;                          //!< vertikale Position der Spins
    int num_neighbors;                          //!< Anzahl der Nachbarn
    gs_edge_t *neighbors;              //!< Pointer auf Array der Kanten
} gs_node_t;

/*! \struct gs_graph_t;
    \brief Ein Graph

    Er besteht aus der Anzahl seiner Knoten und einem Array, in dem Pointer
    auf seine Knoten liegen.
*/
typedef struct
{
    int         num_nodes;    //!< Anzahl der Knoten, Normal \f$ L^2 \f$
    int                 L;                              //!< Seitenlänge
    double              T;                   //!< Temperatur des Systems
    double              M;               //!< Magnetisierung des Systems
    double              E;                      //!< Energie des Systems
    gs_node_t       *node;             //!< Pointer auf ein Array Knoten
} gs_graph_t;

void gs_insert_edge(gs_graph_t *g, int from, int to, double weight);
gs_graph_t *gs_create_graph(int L);
gs_graph_t *gs_copy_graph(gs_graph_t *g);
void print_graph_svg(gs_graph_t *g, char* svg_filename);
void gs_clear_graph(gs_graph_t *g);
int gs_edge_exists(gs_graph_t *g, int from, int to);

int point_not_in_domain(double x, double  y, int L);

#endif /* _GRAPH_H */
