/*! \file spin.h
 * Kompiliere mit -DINCREMENTAL um bei jedem Monte Carlo Schritt die
 * Magnetisierung zu updaten
 */
#ifndef _SPIN_H
#define _SPIN_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "graph.h"
#include "list.h"

double gauss(double sigma);
void create_edges_regular(gs_graph_t *g);
void init_spins_randomly(gs_graph_t *g);
double calculate_energy(gs_graph_t *g);
double calculate_magnetisation(gs_graph_t *g);
void monte_carlo_sweeps(gs_graph_t *g, int N);

#endif /* _SPIN_H */
