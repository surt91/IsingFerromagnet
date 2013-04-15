/*! \file spin.h */

/*! Kompiliere mit -DTIME, um die Laufzeit eines Jeden Laufs im Verbose
 * Modus anzeigen zu lassen
 */
#ifndef _SPIN_H
#define _SPIN_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <unistd.h>                    //< getopt: Kommandozeilenparser
#include <string.h>

#ifdef TIME
    #include <time.h>                        //< Zur Laufzeitbestimmung
#endif

#include <gsl/gsl_rng.h>             //< gsl: für bessere Zufallszahlen

#include "graph.h"
#include "list.h"
#include "stack.h"

double my_rand();
void smy_rand(int seed);
void free_my_rand();

double gauss(double sigma);
double exponential_decay(double alpha, double x);

void move_graph_nodes(gs_graph_t *g, double (*f)(double), double sigma);
void create_edges_regular(gs_graph_t *g);
void assign_weights_with_function(gs_graph_t *g, double (*f)(double alpha, double dist), double alpha);

void init_spins_randomly(gs_graph_t *g);
void init_spins_up(gs_graph_t *g);

double calculate_energy(gs_graph_t *g);
double calculate_magnetisation(gs_graph_t *g);

void metropolis_monte_carlo_sweeps(gs_graph_t *g, int N);
void wolff_monte_carlo_sweeps(gs_graph_t *g, int N);



#endif /* _SPIN_H */
