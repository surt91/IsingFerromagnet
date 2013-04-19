/*! \file spin.h */

/*! \def _XOPEN_SOURCE
 * Damit Barriers in pthread verfügbar werden
 */
#define _XOPEN_SOURCE 600

/*! \def MAX_LEN_FILENAME
 * Maximale Länge des Outputdateinamens
 */
#define MAX_LEN_FILENAME 80

/*! \def THREADED
 * Soll in mehreren Threads gerechnet werden?
 * Kompiliere mit -DTHREADED wenn nicht
 */

#ifndef _SPIN_H
#define _SPIN_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <unistd.h>                    //< getopt: Kommandozeilenparser
#include <string.h>
#include <pthread.h>                             //< Für Multithreading

#include <gsl/gsl_rng.h>             //< gsl: für bessere Zufallszahlen
#include <gsl/gsl_randist.h>            //< gsl: für Gausszufallszahlen

#include "graph.h"
#include "stack.h"

/*! \struct options_t;
    \brief Optionen für die MC Simulation, die per Kommandozeilenargument
            übergeben wurden.
*/
typedef struct
{
    int L;                                  //!< Kantenlänge des Gitters
    int N;                        //!< Anzahl der zu berechnenden Sweeps
    int t_eq;                            //!< geschätze Equilibriumszeit
    double (*moving_fkt)(const gsl_rng *, double);     //!< Unordnungsfunktion
    double sigma;                               //!< Unordnungsparameter
    double (*weighting_fkt)(double, double);    //!< Gewichtungsfunktion
    double alpha;                               //!< Gewichtungsparamter
    int start_order;                                   //!< Startordnung
    void (*mc_fkt)(gs_graph_t *, gsl_rng *);//!< Monte Carlo Algorithmus
    int par_temp_flag;      //!< soll parallel Tempering genutzt werden?
    int num_temps;            //!< Wieviele Temperaturen für pT gegeben?
    double *list_of_temps;                //!< Liste der pT Temperaturen
    int verbose;                                 //!< Gesprächiger Modus
    char filename[MAX_LEN_FILENAME];               //!< Output Dateiname
    gsl_rng *rng;                           //!< Random Number Generator

} options_t;

options_t get_cl_args(int argc, char *argv[]);

double exponential_decay(const double alpha, const double x);

gs_graph_t **init_graphs(const options_t options);
void move_graph_nodes(gs_graph_t *g, double (*f)(const gsl_rng *, double), gsl_rng *rng, const double sigma);
void create_edges_regular(gs_graph_t *g);
void assign_weights_with_function(gs_graph_t *g,
                double (*f)(const double alpha, const double dist),
                                                    const double alpha);

void init_spins_randomly(gs_graph_t *g, gsl_rng *rng);
void init_spins_up(gs_graph_t *g);

double calculate_energy(const gs_graph_t *g);
double calculate_magnetisation(const gs_graph_t *g);

void do_mc_simulation(gs_graph_t **list_of_graphs, const options_t options);
void metropolis_monte_carlo_sweeps(gs_graph_t *g, gsl_rng *rng);
void wolff_monte_carlo_cluster(gs_graph_t *g, gsl_rng *rng);
void par_temp(gs_graph_t **list_of_graphs, int *map_of_temps,
            const options_t o, double *par_temp_versuche, double *par_temp_erfolge);

void write_data_to_file(FILE *data_out_file, gs_graph_t **list_of_graphs,
                     int *map_of_temps, const options_t o, const int N);

#endif /* _SPIN_H */
