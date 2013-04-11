/*! \file spin.c
 * Diese Datei beinhaltet alle Funktionen für eine
 * Metropolis Monte Carlo Simulation bzw. in Zukunft den Wolff
 * Algorithmus und parallel Tempering */
#include "spin.h"

/*! \fn int main(int argc, char *argv[])
    \brief Dies ist die Main Funktion.

    Sie kümmert sich um
    -# das Parsen von Kommandozeilenargumenten
        - -h  -> zeigt die Hilfe
        - -Tx -> Temperatur x                      (double)
        - -Lx -> Länge x                              (int)
        - -xx -> seed x                               (int)
        - -Nx -> x Monte Carlo sweeps                 (int)
        - -ex -> Equilibrium nach x sweeps angenommen (int)
        - -sx -> sigma x                           (double)
        - -ox -> filename (max. 79 Zeichen)        (string)
        - -ux -> Ordnung x (0: zufällig, 1: alle up)  (int)
    -# das Initialisieren des Graphens, auf dem die Spins liegen
    -# das Aufrufen der Funktionen, die die Monte Carlo Sweeps durchführen
    -# das Speichern der Ergebnisse in Dateien

    \param [in] argc     Anzahl Kommandozeilen Argumente
    \param [in] argv     Vektor der Kommandozeilenargumente
*/
int main(int argc, char *argv[])
{
    gs_graph_t *g;
    int i;
    int L;                                   //< Kantenlänge des Gitters
    double sigma;                              //< Unordnung des Gitters
    double E, M, T;
    int N, inc, t_eq;
    int seed, start_order;
    FILE *data_out_file;
    char filename[80];                   //< Dateiname, der Output Datei

    /* Vars für getopt (Kommandozeilenparser) */
    int c;
    extern char *optarg;

    /* Standardwerte, wenn keine Optionen gegeben */
    /* Temperatur */
    T=2.0;
    /* Kantenlänge des Feldes */
    L=64;
    /* Wieviele Sweeps berechnen */
    /* Ein Sweep sind \f$ L^2 \f$ Monte Carlo Schritte */
    N=2000;
    /* nach wie vielen Sweeps ist das Equilibrium erreicht (vgl. t_eq.dat) */
    t_eq = 1000;
    /* Alle wieviel Sweeps Ergebnisse Speichern */
    inc=1;
    /* Parameter, der die Verschiebung der einzelnen Knoten bestimmt */
    sigma = 0;
    /* Anfangsbedingung der Spins: 0: zufällig, 1: alle up */
    start_order = 0;
    /* Seed für Zufallsgenerator */
    seed = 42;
    /* standard Dateiname */
    snprintf(filename, 80, "data/data_T_%.2f_L_%d_up.dat", T, L);

    /* Hier wird die Kommandozeile geparst. */
    /* -Tx -> Temperatur x */
    /* -Lx -> Länge x */
    /* -sx -> seed x */
    /* -Nx -> x Monte Carlo sweeps */
    /* -ex -> Equilibrium nac x sweeps angenommen */
    /* -sx -> sigma x */
    /* -ox -> Dateiname x */
    /* -ux -> Start Ordnung x (0: zufällig, 1: alle up) */
    opterr = 0;
    while ((c = getopt (argc, argv, "hT:L:x:N:e:s:o:u:")) != -1)
        switch (c)
        {
            case 'T':
                T = atof(optarg);
                break;
            case 'L':
                L = atoi(optarg);
                break;
            case 'x':
                seed = atoi(optarg);
                break;
            case 'N':
                N = atoi(optarg);
                break;
            case 'e':
                t_eq = atoi(optarg);
                break;
            case 's':
                sigma = atof(optarg);
                break;
            case 'o':
                strncpy(filename, optarg, 80);
                break;
            case 'u':
                start_order = atoi(optarg);
                break;
            case '?':
                fprintf(stderr,
                        "Unknown option character `\\x%x'.\n", optopt);
            case 'h':
                fprintf(stderr,"Benutzung: %s -[hTLxNesou]\n",argv[0]);
                fprintf(stderr,"    -h     Zeigt diese Hilfe                         \n");
                fprintf(stderr,"    -Tx    Temperatur x                      (double)\n");
                fprintf(stderr,"    -Lx    Länge x                              (int)\n");
                fprintf(stderr,"    -xx    seed x                               (int)\n");
                fprintf(stderr,"    -Nx    x Monte Carlo sweeps                 (int)\n");
                fprintf(stderr,"    -ex    Equilibrium nach x sweeps angenommen (int)\n");
                fprintf(stderr,"    -sx    sigma x                           (double)\n");
                fprintf(stderr,"    -ox    filename (max. 79 Zeichen)        (string)\n");
                fprintf(stderr,"    -ux    Ordnung x (0: zufällig, 1: alle up)  (int)\n");
                return(-1);
            default:
                abort ();
        }

    smy_rand(seed);


    /* Initialisiere den Graphen für die Spins */
    g = gs_create_graph(L);

    /* Verschiebe die Knoten */
    move_graph_nodes(g, gauss, sigma);

    /* Verknüpfe die Knoten */
    create_edges_regular(g);

    /* initialisiere den Status der Spins */
    if(start_order == 1)
        init_spins_up(g);
    else if(start_order == 0)
        init_spins_randomly(g);
    else
        init_spins_randomly(g);

    //~ print_graph_for_graph_viz(g);

    /* Berechne Energie des Ising Modells */
    E = calculate_energy(g);
    //fprintf(stderr, "E = %f\n", E);
    /* Berechne Magnetisierung des Ising Modells */
    M = calculate_magnetisation(g);
    //fprintf(stderr, "M = %f\n", M);
    /* Setze M und E */
    g->M = M;
    g->E = E;

    /* Setze Temperatur */
    g->T = T;

    /* Führe Monte Carlo Sweeps durch */
    /* Erreiche das Equilibrium */
    /* Schreibe alle inc Sweeps die Energie und Magnetisierung in eine Datei */
    /* Plotte den Ausdruck mit Gnuplot. zB.
     * plot 'test.dat' using 1:2, "test.dat" using 1:3; */
    data_out_file = fopen(filename, "w");
    if(data_out_file == NULL)
    {
        fprintf(stderr,"ERROR: %s kann nicht geöffnet werden",filename);
        return(-1);
    }
    fprintf(data_out_file, "#N E M\n");
    for(i=0;i<N;i+=inc)
    {
        monte_carlo_sweeps(g, inc);
        if(i > t_eq)
            fprintf(data_out_file, "%d %f %f\n",i, g->E, fabs(g->M)/g->num_nodes);
    }
    fclose(data_out_file);

    gs_clear_graph(g);
    free_my_rand();

    return(0);
}

/*! \fn double wrapper_for_gsl_rand(int set_seed, int seed, int free)
    \brief Eine Funktion, die die GSL rand Funktionen in ein gewohntes Format
            überführt

    \param [in] set_seed Ob ein neuer Seed gesetzt werden soll
    \param [in] seed     Neuer Seed
    \param [in] free     Ob der rng Speicher freigegeben werden soll
*/
double wrapper_for_gsl_rand(int set_seed, int seed, int free)
{
    static gsl_rng * rng;
    const gsl_rng_type * T;

    if(set_seed)
    {
        gsl_rng_env_setup();

        T = gsl_rng_mt19937;
        rng = gsl_rng_alloc (T);
        gsl_rng_set(rng, seed);
        return(0);
    }
    else if(free)
    {
        gsl_rng_free(rng);
        return(0);
    }
    else
        return(gsl_rng_uniform (rng));
}
/*! \fn my_rand()
    \brief Erzeugt eine gleichverteilte Zufallszahl

    \return Gleichverteilte Zufallszahl aus [0,1)
*/
double my_rand()
{
    return(wrapper_for_gsl_rand(0, 0, 0));
}
/*! \fn void smy_rand(int seed)
    \brief Initialisiert den Zufallszahlengenerator mit einem Seed

    \param [in] seed Seed
*/
void smy_rand(int seed)
{
    wrapper_for_gsl_rand(1, seed, 0);
}
/*! \fn void free_my_rand()
    \brief Gibt den vom Zufallszahlengenerator beanspruchten Speicher frei
*/
void free_my_rand()
{
    wrapper_for_gsl_rand(0, 0, 1);
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
    u1 = my_rand();
    u2 = my_rand();

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
        if(my_rand()<0.5)
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
    int sk;                  /*< Wert des k-ten Spins (temporäre Var) */
    double E=0;                               /*< Energie des Systems */
    double E_sk;          /*< Energie des k-ten Spins (temporäre Var) */
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
    return(-E/2);   /* Da über alle Spins zweimal summiert wurde: E/2 */
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
    int delta_E;
    double A;
    elem_t *list;
    /* Array mit vorberechneten exp(E/T)
     * Es gibt die möglichen delta_E Werte -8, -4, 0, 4, 8
     * Nur die positiven müssen berechnet werden.
     */
    static double exp_T[2];
    static int exp_ready=0;

    if(!exp_ready)
    {
        exp_T[0] = exp(-4/g->T);
        exp_T[1] = exp(-8/g->T);
        exp_ready = 1;
    }

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes*N;i++)
    {
        delta_E = 0;
        /*! - Ermittele den zu flippenden Spin zufällig. */
        to_flip_idx = floor(my_rand()*num_nodes);
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
/*        printf("%f\n",delta_E);*/
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
        switch(delta_E)
        {
            case 4:
                A = exp_T[0];
                break;
            case 8:
                A = exp_T[1];
                break;
            default:
                A = 1.0;
                break;
        }
        if(A > my_rand())
        {
            /* drehe den Spin um */
            g->node[to_flip_idx].spin *= -1;

            g->E += delta_E;

            g->M += 2 * g->node[to_flip_idx].spin;
        }
    }
}
