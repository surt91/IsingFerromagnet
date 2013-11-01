#include "correlation_by_k_susceptibility.h"

double calculate_wave_vector_dependend_susceptibility_1D(gs_graph_t *g, double k)
{
    int i;
    gsl_complex tmp_sum = gsl_complex_rect (0, 0);
    gsl_complex img_unit = gsl_complex_rect (0, 1);
    gsl_complex z;

    for(i=0;i<g->num_nodes;i++)
    {
        z = gsl_complex_mul_real(img_unit, k*g->node[i].x);
        tmp_sum = gsl_complex_add(tmp_sum, gsl_complex_mul_real(gsl_complex_exp(z), g->spins[i]));
    }
    tmp_sum = gsl_complex_div_real(tmp_sum, g->num_nodes);
    tmp_sum = gsl_complex_pow_real(tmp_sum, 2);

    return GSL_REAL(tmp_sum);
    //~ return gsl_complex_abs(tmp_sum);
}
