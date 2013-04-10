#include "spin.h"

/*! \def INV_SQRT2PI
    \brief Konstante: \f$ \frac{1}{sqrt(2 \pi)} \f$ für Gaussfunktion laut Wolfram|alpha
*/
#define INV_SQRT2PI 0.39894228040143267793994605993438

int main()
{
    gs_graph_t *g;
    int i;
    int L;                                  //!< Kantenlänge des Gitters
    double sigma;//!< Parameter, der die Verschiebung der einzelnen Knoten bestimmt
    double E, M;
    int N, inc;
    FILE *data_out_file;
    char filename[80];                  //!< Dateiname, der Output Datei

    srand(100);
    srand48(100);

    /* Kantenlänge des Feldes */
    L=16;
    /* Wieviele Sweeps berechnen */
    /* Ein Sweep sind \f$ L^2 \f$ Monte Carlo Schritte */
    N=4000;
    /* Alle wieviel Sweeps Ergebnisse Speichern */
    inc=1;
    /* Parameter, der die Verschiebung der einzelnen Knoten bestimmt */
    sigma = 0;

    /* Initialisiere den Graphen für die Spins */
    g = gs_create_graph(L);

    /* Verschiebe die Knoten */
    move_graph_nodes(g, gauss, sigma);

    /* Verknüpfe die Knoten */
    create_edges_regular(g);

    /* initialisiere den Status der Spins */
    init_spins_randomly(g);
    /* init_spins_up(g); */
    //~ print_graph_for_graph_viz(g);

    /* Berechne Energie des Ising Modells */
    E = calculate_energy(g);
    fprintf(stderr, "E = %f\n", E);
    /* Berechne Magnetisierung des Ising Modells */
    M = calculate_magnetisation(g);
    fprintf(stderr, "M = %f\n", M);
    /* Setze M und E */
    g->M = M;
    g->E = E;

    /* Setze Temperatur */
    g->T = 2;

    /* Führe Monte Carlo Sweeps durch */
    /* Schreibe alle 10 Sweeps die Energie und Magnetisierung in eine Datei */
    /* Plotte den Ausdruck mit Gnuplot. zB.
     * plot 'test.dat' using 1:2, "test.dat" using 1:3; */
    snprintf(filename, 80, "data_T_%.2f.dat", g->T);
    data_out_file = fopen(filename, "w");           /* Fehlerbehandlung fehlt */
    fprintf(data_out_file, "#N E M\n");
    for(i=0;i<N;i+=inc)
    {
        monte_carlo_sweeps(g, inc);
        fprintf(data_out_file, "%d %f %f\n",i, g->E, g->M);
    }

    /* Berechne Energie des Ising Modells */
    E = calculate_energy(g);
    fprintf(stderr, "E = %f, inc: %f\n", E, g->E);
    /* Berechne Magnetisierung des Ising Modells */
    M = calculate_magnetisation(g);
    #ifdef INCREMENTAL
        fprintf(stderr, "M = %f, inc: %f\n", M, g->M);
    #else
        fprintf(stderr, "M = %f\n", M);
    #endif
    print_graph_for_graph_viz(g);

    gs_clear_graph(g);

    return(0);
}

/*! \fn double gauss(double sigma)
    \brief Erzeugt Gauss verteilte Zufallszahlen nach der Box-Müller
            Methode.

    Siehe auch \cite hartmann2009practical S. 250, 7.2.5

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

    /* passe n1 entsprechend mu und sigma an */
    n1 = n1*sigma+mu;

    return(n1);
}

/*! \fn void move_graph_nodes(gs_graph_t *g, double (*f)(double), double sigma)
    \brief Verschiebt die Knoten des Graphen g, wobei die Verschiebung zufällig
            nach der Verteilung f mit dem Parameter sigma gewählt wird.

    \param [in,out]   g    Graph, dessen Knoten verschoben werden sollen
    \param [in]       f    Funktion, die Zufallszahlen entsprechend der
                           gewünschten Verteilung liefert
    \param [in]   sigma    Paramter der Verteilung (zb. Standardabweichung)
*/
void move_graph_nodes(gs_graph_t *g, double (*f)(double), double sigma)
{
    int n;
    for(n=0;n<g->num_nodes; n++)
    {
        g->node[n].x += f(sigma);
        g->node[n].y += f(sigma);
    }
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

        if(i+L < num_nodes)
            gs_insert_edge(g, i, i+L, weight);  /*oben*/
        else
            gs_insert_edge(g, i, i%L, weight);
    }
}

/*! \fn void init_spins_randomly(gs_graph_t *g)
    \brief Weist den Spins im gs_graph_t g zufällige Werte zu

    \param [in,out]    g    Graph, der modifiziert werden soll
*/
void init_spins_randomly(gs_graph_t *g)
{
    int num_nodes, i;

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes;i++)
    {
        /* Todo: andere rand() Funktion (aus gsl?) */
        if(drand48()<0.5)
            g->node[i].spin = 1;
        else
            g->node[i].spin = -1;
    }
}

/*! \fn void init_spins_up(gs_graph_t *g)
    \brief Weist den Spins im gs_graph_t g den Wert Up = 1 zu

    \param [in,out]    g    Graph, der modifiziert werden soll
*/
void init_spins_up(gs_graph_t *g)
{
    int num_nodes, i;

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes;i++)
        g->node[i].spin = 1;
}

/*! \fn void calculate_energy(gs_graph_t *g)
    \brief Berechnet die Energie des Ising Modells

    Dies wird durch einfache Auswertung des Hamiltonoperators des
    Ising Modells erledigt. \f[ E = H = -\sum_{<ij>} J_{ij} s_i s_j \f]
    Dabei ist J die Kopplungskonstante, hier also das Kantengewicht.

    \param [in,out]    g    Graph, der modifiziert werden soll
    \return Energie des Systems.
*/
double calculate_energy(gs_graph_t *g)
{
    int num_nodes, i;
    int sk;                 /*!< Wert des k-ten Spins (temporäre Var) */
    double E=0;                              /*!< Energie des Systems */
    double E_sk;         /*!< Energie des k-ten Spins (temporäre Var) */
    elem_t *list;

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes;i++)                      /* Über alle Knoten */
    {
        E_sk = 0;
        sk = g->node[i].spin;
        list = g->node[i].neighbors;
        while(list != NULL)                     /* Über alle Nachbarn */
        {
            E_sk += g->node[list->index].spin * list->weight;
            list = list->next;
        }
        E_sk *= sk;
        E += E_sk;
    }
    return(-E/2);    /* Da über alle Spins zweimal summiert wurde: E/2 */
}

/*! \fn void calculate_magnetisation(gs_graph_t *g)
    \brief Berechnet die Magnetisierung des Ising Modells

    Dies wird durch einfache Auswertung der Definition erledigt.
    \f[ M = \sum_{i} s_i \f]

    \param [in,out]    g    Graph, der modifiziert werden soll
    \return Magnetisierung des Systems.
*/
double calculate_magnetisation(gs_graph_t *g)
{
    int num_nodes, i;
    double M=0;

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes;i++)                      /* Über alle Knoten */
        M += g->node[i].spin;

    return(M);
}

/*! \fn double monte_carlo_sweeps(gs_graph_t *g, int N)
    \brief Führt N Monte Carlo Sweeps durch

    Dabei besteht ein Sweep aus \f$ L^2 \f$ Monte Carlo Schritten

    \param [in,out]    g    Graph, der modifiziert werden soll
    \param [in]        N    Anzahl der zu berechnenden Sweeps
    \return Magnetisierung des Systems.
*/
void monte_carlo_sweeps(gs_graph_t *g, int N)
{
    int i;
    int num_nodes;
    int to_flip_idx;
    double delta_E;
    double A;
    elem_t *list;

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes*N;i++)
    {
        delta_E = 0;
        /*! - Ermittele den zu flippenden Spin zufällig. */
        to_flip_idx = floor(drand48()*num_nodes);
        /*! - Ermittele Energieänderung
            \f[ \Delta E = 2s_k \sum_{\mathrm{adj}(s_k)} J_{kj} s_j \f]
            So wie in \cite newman1999monte S. 52 (3.10) gegeben,
            Jedoch leicht abgewandelt, um verschiedene J zu
            berücksichtigen.
         */
        list = g->node[to_flip_idx].neighbors;
        while(list != NULL)
        {
            /* Hier wird die Summe ausgeführt: E += s_j * J_{ij} */
            delta_E += g->node[list->index].spin * list->weight;
            list = list->next;
        }
        /* Hier werden die Koeffizienten berücksichtigt: E = 2s_k*sum */
        delta_E *= 2 * g->node[to_flip_idx].spin;
        /*! - Berechne die Wahrscheinlichkeit, mit der der Flip akzeptiert
            wird.
            \f[ A = \left\{
                      \begin{array}{ll}
                        e^{-\beta \Delta E} & \Delta E > 0 \\
                        1 & sonst
                      \end{array}
                    \right.  \f]
            Wie in \cite newman1999monte S. 49 (3.7) gegeben.
         */
        if(delta_E > 0)
            A = exp(-delta_E/g->T);
        else
            A = 1.0;
        if(A > drand48())
        {
            /* drehe den Spin um */
            g->node[to_flip_idx].spin *= -1;

            g->E += delta_E;

            #ifdef INCREMENTAL
                g->M += 2 * g->node[to_flip_idx].spin;
            #endif
        }
    }
}
