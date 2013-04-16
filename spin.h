/*! \file spin.h */

/*! \def MAX_LEN_FILENAME
 * Maximale Länge des Outputdateinamens
 */
#define MAX_LEN_FILENAME 80

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

#include <gsl/gsl_rng.h>             //< gsl: für bessere Zufallszahlen

#include "graph.h"
#include "list.h"
#include "stack.h"

double my_rand();
void smy_rand(int seed);
void free_my_rand();

void get_cl_args(int argc, char *argv[], int *L, double *T, int *N,
                    int *t_eq, int *inc, double *sigma, double *alpha,
                    int *start_order, int *wolff_flag,
                    int *par_temp_flag, int *num_temps, int *verbose,
                    char (*filename)[MAX_LEN_FILENAME], double **list_of_temps);

double gauss(double sigma);
double exponential_decay(double alpha, double x);

gs_graph_t **init_graphs(int L, int num_temps, double *list_of_temps, int start_order,
                        double (*moving_fkt)(double), double sigma,
                        double (*weighting_fkt)(double alpha, double dist), double alpha);
void move_graph_nodes(gs_graph_t *g, double (*f)(double), double sigma);
void create_edges_regular(gs_graph_t *g);
void assign_weights_with_function(gs_graph_t *g, double (*f)(double alpha, double dist), double alpha);

void init_spins_randomly(gs_graph_t *g);
void init_spins_up(gs_graph_t *g);

double calculate_energy(gs_graph_t *g);
double calculate_magnetisation(gs_graph_t *g);

void do_mc_simulation(gs_graph_t **list_of_graphs, int N, int inc, int num_temps,
                                int t_eq, int par_temp_flag, int wolff_flag,
                                char filename[MAX_LEN_FILENAME], int verbose);
void metropolis_monte_carlo_sweeps(gs_graph_t *g, int N);
void wolff_monte_carlo_sweeps(gs_graph_t *g, int N);

#endif /* _SPIN_H */
