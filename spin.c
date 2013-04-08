#include "spin.h"

/*! \def INV_SQRT2PI
    \brief Konstante: \f$ \frac{1}{sqrt(2 \pi)} \f$ für Gaussfunktion laut Wolfram|alpha
*/
#define INV_SQRT2PI 0.39894228040143267793994605993438

int main()
{
    gs_graph_t *g;
    int L;                                  //!< Kantenlänge des Gitters
    double sigma = 0.1;

    L=3;

    g = gs_create_graph(L, gauss, sigma);
    create_edges_regular(g);
    print_graph_for_graph_viz(g);

    gs_clear_graph(g);

    return(0);
}

/*! \fn double gauss(double sigma)
    \brief Erzeugt Gauss verteilte Zufallszahlen nach der Box-Müller
            Methode.

    Siehe auch \cite hartmann2009practical S. XXX (Y.Z)

    \param [in]   sigma    Standardabweichung der Glockenkurve
    \return Gaussverteilte Zufallszahl
*/
double gauss(double sigma)
{
    double mu = 0;
    double u1, u2;
    double n1;

    /* hier eventuell andere Zufallsgeneratoren benutzen */
    u1 = drand48();
    u2 = drand48();

    n1 = sqrt(-2*log(1-u1))*cos(2*M_PI*u2);

    /* passe n1 und n2 entsprechend mu und sigma an */
    n1 = n1*sigma+mu;

    return(n1);
}

/*! \fn void create_edges_regular(gs_graph_t *g)
    \brief Fügt Kanten zu einem Graphen hinzu, wodurch ein reguläres
            quadratisches Gitter erzeugt werden soll. Kantengewichte (J)
            sind alle 1.

    \param [in,out]    g    Graph, der modifiziert werden soll
*/
void create_edges_regular(gs_graph_t *g)
{
    int i;
    int num_nodes, L;
    double weight;

    weight = 1.0;

    num_nodes = g->num_nodes;
    L         = g->L;

    for(i=0;i<num_nodes;i++)
    {
        if(i%L < L-1)
            gs_insert_edge(g, i, i+1, weight); /*rechts*/
        else
            gs_insert_edge(g, i, i/L *L, weight);

        //~ if(i%L > 0)
            //~ gs_insert_edge(g, i, i-1, weight);  /*links*/
        //~ else
            //~ gs_insert_edge(g, i, i/L *L+L-1, weight);

        if(i+L < num_nodes)
            gs_insert_edge(g, i, i+L, weight);  /*oben*/
        else
            gs_insert_edge(g, i, i%L, weight);

        //~ if(i-L >= 0)
            //~ gs_insert_edge(g, i, i-L, weight);  /*unten*/
        //~ else
            //~ gs_insert_edge(g, i, num_nodes - L + i%L , weight);
    }
}
