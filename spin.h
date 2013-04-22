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
#include "list.h"

/*! \def MIN(X,Y)
 * Minimum von zwei Zahlen
 */
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
/*! \def MAX(X,Y)
 * Maximum von zwei Zahlen
 */
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

/*! \struct options_t;
    \brief Optionen für die MC Simulation, die per Kommandozeilenargument
            übergeben wurden.
*/
typedef struct
{
    int L;                                  //!< Kantenlänge des Gitters
    int N;                        //!< Anzahl der zu berechnenden Sweeps
    int t_eq;                            //!< geschätze Equilibriumszeit
    double (*moving_fkt)(const gsl_rng *, double); //!< Unordnungsfunkt.
    double sigma;                               //!< Unordnungsparameter
    double (*weighting_fkt)(double, double);    //!< Gewichtungsfunktion
    double alpha;                               //!< Gewichtungsparamter
    int start_order;                                   //!< Startordnung
    int seed;                                                  //!< Seed
    int tau;                                   //!< Autokorrelationszeit
    int (*graph_fkt)(double, gs_node_t, gs_node_t, gs_node_t); //!<Graph
    void (*graph_cell_border_fkt)(gs_node_t, gs_node_t, double,
                                     int*, int*, int*, int*); //!< Graph
    void (*mc_fkt)(gs_graph_t *, gsl_rng *);//!< Monte Carlo Algorithmus
    int par_temp_flag;      //!< soll parallel Tempering genutzt werden?
    int num_temps;            //!< Wieviele Temperaturen für pT gegeben?
    double *list_of_temps;                //!< Liste der pT Temperaturen
    int verbose;                                 //!< Gesprächiger Modus
    int zip;                                        //!< gzip Outputfile
    char filename[MAX_LEN_FILENAME];               //!< Output Dateiname
    char svg_filename[MAX_LEN_FILENAME];     //!< Graph Output Dateiname
    gsl_rng *rng;                           //!< Random Number Generator

} options_t;

options_t get_cl_args(int argc, char *argv[]);

double exponential_decay(const double alpha, const double x);

gs_graph_t **init_graphs(const options_t options);
void move_graph_nodes(gs_graph_t *g, double (*f)(const gsl_rng *, double), gsl_rng *rng, const double sigma);

inline int check_relative_neighborhood(double dist12, gs_node_t node1, gs_node_t node2, gs_node_t node3);
inline void get_cell_border_relative_neighborhood(gs_node_t node1, gs_node_t node2, double dist12, int *x0, int *x1, int *y0, int *y1);
inline int check_gabriel(double dist12, gs_node_t node1, gs_node_t node2, gs_node_t node3);
inline void get_cell_border_gabriel(gs_node_t node1, gs_node_t node2, double dist12, int *x0, int *x1, int *y0, int *y1);
void create_edges(gs_graph_t *g, options_t o);

void init_spins_randomly(gs_graph_t *g, gsl_rng *rng);
void init_spins_up(gs_graph_t *g);

double calculate_energy(const gs_graph_t *g);
double calculate_magnetisation(const gs_graph_t *g);

void do_mc_simulation(gs_graph_t **list_of_graphs, const options_t options);
void metropolis_monte_carlo_sweep(gs_graph_t *g, gsl_rng *rng);
void wolff_monte_carlo_cluster(gs_graph_t *g, gsl_rng *rng);
void metroplis_sweep_and_wolff_cluster(gs_graph_t *g, gsl_rng *rng);
void par_temp(gs_graph_t **list_of_graphs, int *map_of_temps,
            const options_t o, double *par_temp_versuche, double *par_temp_erfolge);

void write_data_to_file(FILE *data_out_file, gs_graph_t **list_of_graphs,
                     int *map_of_temps, const options_t o, const int N);

#endif /* _SPIN_H */
