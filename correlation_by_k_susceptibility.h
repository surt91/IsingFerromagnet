/*! \file */
#ifndef _CORRELATION_BY_K_SUSCEPTIBILITY_H
#define _CORRELATION_BY_K_SUSCEPTIBILITY_H

#include <math.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_complex.h>
#include "graph.h"

double calculate_wave_vector_dependend_susceptibility_1D(gs_graph_t *g, double k);

#endif /* _CORRELATION_BY_K_SUSCEPTIBILITY_H */
