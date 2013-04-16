/*! \file spin.c
 * Diese Datei beinhaltet alle Funktionen für eine
 * Metropolis Monte Carlo Simulation bzw. in Zukunft den Wolff
 * Algorithmus und parallel Tempering */
#include "spin.h"

/*! \def MAX_LEN_FILENAME
 * Maximale Länge des Outputdateinamens
 */
#define MAX_LEN_FILENAME 80

/*! \fn int main(int argc, char *argv[])
    \brief Dies ist die Main Funktion.

    Sie kümmert sich um
    -# das Parsen von Kommandozeilenargumenten
        - -v     zeigt die Hilfe
        - -h     gesprächiger Modus
        - -Tx    Temperatur x                      (double)
        - -Lx    Länge x                              (int)
        - -xx    seed x                               (int)
        - -Nx    x Monte Carlo sweeps                 (int)
        - -ex    Equilibrium nach x sweeps angenommen (int)
        - -sx    sigma x                           (double)
        - -ox    filename (max. 79 Zeichen)        (string)
        - -ux    Ordnung x (0: zufällig, 1: alle up)  (int)
        - -ix    Alle x sweeps schreiben              (int)
        - -w     Wolff Algorithmus (statt Metropolis)
        - -px    Parallel Tempering mit einer Komma
                 getrennten Liste der zu berechnenden Temperaturen
    -# das Initialisieren des Graphens, auf dem die Spins liegen
    -# das Aufrufen der Funktionen, die die Monte Carlo Sweeps durchführen
    -# das Speichern der Ergebnisse in Dateien

    \param [in] argc     Anzahl Kommandozeilen Argumente
    \param [in] argv     Vektor der Kommandozeilenargumente
*/
int main(int argc, char *argv[])
{
    gs_graph_t *g;
    gs_graph_t **list_of_graphs;
    int i, j, nT;
    int L;                                   //< Kantenlänge des Gitters
    double sigma;                              //< Unordnung des Gitters
    double alpha;                              //< Gewichtung der Kanten
    double T;
    double *list_of_temps;
    double tmp_T, delta;
    int num_temps, temp_index;                //< Für parallel Tempering
    char temp_string[20];
    int N, inc, t_eq;
    int seed, start_order;
    double *par_temp_versuche, *par_temp_erfolge;
    FILE *data_out_file;
    char filename[MAX_LEN_FILENAME];     //< Dateiname, der Output Datei

    #ifdef TIME
        double begin_time, end_time;          //< for timing Information
    #endif

    /* Vars für getopt (Kommandozeilenparser) */
    int c, verbose=0, custom_file_name=0;
    int wolff_flag, par_temp_flag;
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
    /* Parameter, der die Gewichtung der Kanten bestimmt */
    alpha = 0;
    /* Anfangsbedingung der Spins: 0: zufällig, 1: alle up */
    start_order = 0;
    /* Seed für Zufallsgenerator */
    seed = 42;
    /* Soll Wolff Algorithmus benutzt werden? */
    wolff_flag = 0;
    /* Soll Parallel Tempering Algorithmus benutzt werden? */
    par_temp_flag = 0;
    num_temps = 1;

    /* Hier wird die Kommandozeile geparst. */
    /* -h  -> help */
    /* -v  -> verbose */
    /* -Tx -> Temperatur x */
    /* -Lx -> Länge x */
    /* -sx -> seed x */
    /* -Nx -> x Monte Carlo sweeps */
    /* -ex -> Equilibrium nac x sweeps angenommen */
    /* -sx -> sigma x */
    /* -ox -> Dateiname x */
    /* -ux -> Start Ordnung x (0: zufällig, 1: alle up) */
    /* -ix -> Alle x sweeps schreiben */
    /* -w  -> Wolff Algorithmus (statt Metropolis) */
    /* -px    Parallel Tempering aktivieren, mit den kommagetrennten
     *          Temperauren dahinter */
    opterr = 0;
    while ((c = getopt (argc, argv, "hvT:L:x:N:e:s:o:u:i:wp:")) != -1)
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
                custom_file_name = 1;
                strncpy(filename, optarg, MAX_LEN_FILENAME);
                break;
            case 'u':
                start_order = atoi(optarg);
                break;
            case 'i':
                inc = atoi(optarg);
                break;
            case 'v':
                verbose = 1;
                break;
            case 'w':
                wolff_flag = 1;
                break;
            case 'p':
                par_temp_flag = 1;
                i=0;
                /* Ermittele Anzahl der Temperaturen */
                while(optarg[i]!= '\0')
                    if(optarg[i++] == ',')
                        num_temps++;
                /* Reserviere Speicher */
                list_of_temps = (double*) malloc(num_temps * sizeof(double));
                /* Schreibe die Temperaturen als Double in das Array */
                nT=0; j=0; i=0;
                do
                {
                    if(optarg[i] == ',' || optarg[i] == '\0')
                    {
                        temp_string[j] = '\0';
                        j=0;
                        list_of_temps[nT++] = atof(temp_string);
                    }
                    else
                    {
                        temp_string[j] = optarg[i];
                        j++;
                    }
                } while(optarg[i++]!= '\0');
                break;
            case '?':
                fprintf(stderr,
                        "Unknown option character `\\x%x'.\n", optopt);
            case 'h':
                fprintf(stderr,"Benutzung: %s -[hTLxNesou]\n",argv[0]);
                fprintf(stderr,"    -h     Zeigt diese Hilfe                         \n");
                fprintf(stderr,"    -v     gesprächiger Modus                        \n");
                fprintf(stderr,"    -Tx    Temperatur x                      (double)\n");
                fprintf(stderr,"    -Lx    Länge x                              (int)\n");
                fprintf(stderr,"    -xx    seed x                               (int)\n");
                fprintf(stderr,"    -Nx    x Monte Carlo sweeps                 (int)\n");
                fprintf(stderr,"    -ex    Equilibrium nach x sweeps angenommen (int)\n");
                fprintf(stderr,"    -sx    sigma x                           (double)\n");
                fprintf(stderr,"    -ox    filename (max. 79 Zeichen)        (string)\n");
                fprintf(stderr,"    -ux    Ordnung x (0: zufällig, 1: alle up)  (int)\n");
                fprintf(stderr,"    -ix    Alle x sweeps schreiben              (int)\n");
                fprintf(stderr,"    -w     Wolff Algorithmus (statt Metropolis)      \n");
                fprintf(stderr,"    -px    Parallel Tempering mit einer Komma        \n");
                fprintf(stderr,"            getrennten Liste der zu berechnenden Temperaturen \n");
                return(-1);
            default:
                abort ();
        }

    smy_rand(seed);
    list_of_graphs = (gs_graph_t**) malloc(num_temps * sizeof(gs_graph_t*));

    par_temp_versuche = (double*) calloc(num_temps, sizeof(double));
    par_temp_erfolge  = (double*) calloc(num_temps, sizeof(double));

    if(!par_temp_flag)
    {
        list_of_temps = (double*) malloc(num_temps * sizeof(double));
        list_of_temps[0] = T;
    }
    if(!custom_file_name)
    {
        /* standard Dateiname */
        snprintf(filename, MAX_LEN_FILENAME, "data/data_L_%d.dat", L);
    }

    if(verbose)
    {
        printf("gewählte Parameter:\n");
        printf("    L     = %d\n", L);
        printf("    T     =  \n");
        for(i=0;i<num_temps;i++)
            printf("            %f,\n",list_of_temps[i]);
        printf("           \n");
        printf("    N     = %d\n", N);
        printf("    t_eq  = %d\n", t_eq);
        printf("    seed  = %d\n", seed);
        printf("    sigma = %f\n", sigma);
        if(start_order)
            printf("    spins starten alle up\n");
        else
            printf("    spins starten zufällig\n");
        if(wolff_flag)
            printf("    Wolff Algorithmus\n");
        else
            printf("    Metropolis Algorithmus\n");
        if(par_temp_flag)
            printf("    Parallel Tempering aktiviert\n");
        else
            printf("    Parallel Tempering deaktiviert\n");
        printf("    Filename: '%s'\n",filename);
        printf("    \n");

        #ifdef TIME
            begin_time = clock();
        #endif
    }

    /* Initialisiere den Graphen für die Spins */
    g = gs_create_graph(L);

    /* Verschiebe die Knoten */
    move_graph_nodes(g, gauss, sigma);

    /* Verknüpfe die Knoten */
    create_edges_regular(g);
    assign_weights_with_function(g, exponential_decay, alpha);

    for(nT=0;nT<num_temps;nT++)
    {
        /* initialisiere den Status der Spins */
        if(start_order == 1)
            init_spins_up(g);
        else
            init_spins_randomly(g);

        /* Berechne Energie des Ising Modells */
        g->E = calculate_energy(g);
        /* Berechne Magnetisierung des Ising Modells */
        g->M = calculate_magnetisation(g);

        /* Speichere eine Kopie des Graphen im Array der Graphen */
        list_of_graphs[nT] = gs_copy_graph(g);

        /* Setze Temperatur */
        list_of_graphs[nT]->T = list_of_temps[nT];
    }
    gs_clear_graph(g);

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
    /* Schreibe Header */
    fprintf(data_out_file, "# N E M # L=%d # T= ", L);
    for(nT=0;nT<num_temps;nT++)
        fprintf(data_out_file, "%.3f, ", list_of_graphs[nT]->T);
    fprintf(data_out_file, "\n");

    /* Führe einen Sweep durch */
    for(i=0;i<N;i+=inc)
    {
        if(i > t_eq)
            fprintf(data_out_file, "%d ", i);
        /* Für jede Temperatur */
        for(nT=0;nT<num_temps;nT++)
        {
            g = list_of_graphs[nT];
            /* Welchen Algorithmus nutzen? */
            if(! wolff_flag)
                metropolis_monte_carlo_sweeps(g, inc);
            else
            {
                wolff_monte_carlo_sweeps(g, inc);
                g->E = calculate_energy(g);
            }

            g->M = calculate_magnetisation(g);

            /* Ergebnisse nur in die Ausgabe schreiben, wenn die (vermutete)
                Equilibriumszeit verstrichen ist */
            if(i > t_eq)
                fprintf(data_out_file, "%f %f ", g->E, g->M/g->num_nodes);
        }
        if(i > t_eq)
            fprintf(data_out_file, "\n");
        /* wähle eine zufällige Temperatur und tausche sie mit der nächst höheren */
        /* stelle sicher, dass nie die höchste gewählt wird */

        if(par_temp_flag)
        {
            //~ temp_index = (int) (my_rand() * (num_temps-1));
            for(j=0;j<num_temps-1;j++)
            {
                temp_index = j;
                par_temp_versuche[temp_index]++;
                /* Wähle zufällig, ob getauscht werden soll */
                delta = ( 1/list_of_graphs[temp_index]->T - 1/list_of_graphs[temp_index+1]->T )
                        * ( list_of_graphs[temp_index]->E - list_of_graphs[temp_index+1]->E);
                if(my_rand() < exp(delta))
                {
                    par_temp_erfolge[temp_index]++;
                    /* Tausche erst die Graphen ... */
                    g = list_of_graphs[temp_index];
                    list_of_graphs[temp_index] = list_of_graphs[temp_index+1];
                    list_of_graphs[temp_index+1] = g;
                    /* ... tausche danach die Temperaturen zurück */
                    tmp_T = list_of_graphs[temp_index]->T;
                    list_of_graphs[temp_index]->T = list_of_graphs[temp_index+1]->T;
                    list_of_graphs[temp_index+1]->T = tmp_T;
                }
            }
        }
    }
    if(verbose)
    {
        fprintf(stderr,"Akzeptanzniveaus: \n");
        fprintf(stderr,"T: ");
        for(nT=0;nT<num_temps;nT++)
            fprintf(stderr,"%.2f      ",list_of_temps[nT]);
        fprintf(stderr,"\n");
        fprintf(stderr,"A: ");
        for(nT=0;nT<num_temps;nT++)
            fprintf(stderr,"     %.2f ", par_temp_erfolge[nT]/par_temp_versuche[nT]);
        fprintf(stderr,"\n");
    }

    fclose(data_out_file);

    for(nT=0;nT<num_temps;nT++)
        gs_clear_graph(list_of_graphs[nT]);
    free(list_of_graphs);
    free(list_of_temps);
    free_my_rand();
    free(par_temp_versuche);
    free(par_temp_erfolge);

    #ifdef TIME
        if(verbose)
        {
            end_time = clock();
            printf("Laufzeit\n    %.2f s\n\n",
                         (double)(end_time-begin_time)/CLOCKS_PER_SEC);
        }
    #endif

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

    u1 = my_rand();
    u2 = my_rand();

    n1 = sqrt(-2*log(1-u1))*cos(2*M_PI*u2);

    /* passe n1 entsprechend mu und sigma an */
    n1 = n1*sigma+mu;

    return(n1);
}

/*! \fn double exponential_decay(double alpha, double x)
    \brief Berechnet den exponentiellen Abfall \f$ e^{-\alpha x} \f$

    \param [in]   alpha    Parameter des Abfalls
    \param [in]   x        Argument der Funktion
    \return \f$ e^{-\alpha x} \f$
*/
double exponential_decay(double alpha, double x)
{
    return(exp(-alpha*x));
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

/*! \fn void assign_weights_with_function(gs_graph_t *g, double (*f)(double alpha, double dist), double alpha)
    \brief Ändert die Kantengewichte eines Graphen abhängig vom Abstand
            der Knoten nach einem exponentiellen Zusammenhang.

            Dieser Exponentielle Zusammenhang ist
            \f$ J = e^{-\alpha d} \f$
            wobei d der Abstand er Knoten ist

    \param [in,out]    g     Graph, der modifiziert werden soll
    \param [in]        alpha Gewichtungsfaktor
*/
void assign_weights_with_function(gs_graph_t *g,
                   double (*f)(double alpha, double dist), double alpha)
{
    int i,j;
    int L;
    double weight, dist;
    double dx, dy;
    elem_t *list;

    for(i=0;i<g->num_nodes;i++)
    {
        L = g->L;

        list = g->node[i].neighbors;
        while(list != NULL)                     /* Über alle Nachbarn */
        {
            j=list->index;

            /* !!! */
            /* Funktioniert nur im regulären Gitter: Muss ersetzt werden */
            /* !!! */

            /* Prüfe, ob der Knoten ein rechter oder linker Randknoten ist
                Wenn ja, reduziere den Abstand in x-Richtung um L */
            if(    (i%L == 0   && j%L == L-1)    /* i links, j rechts */
                || (i%L == L-1 && j%L == 0)   )  /* i rechts, j links */
            {
                dx = abs(g->node[j].x - g->node[i].x) - L;
            }
            else
                dx = g->node[j].x - g->node[i].x;

            /* Prüfe, ob der Knoten ein oberer oder unterer Randknoten ist
                Wenn ja, reduziere den Abstand in y-Richtung um L */
            if(    (i/L == 0   && j/L == L-1)    /* i oben, j unten */
                || (i/L == L-1 && j/L == 0)   )  /* i unten, j oben */
            {
                dy = abs(g->node[j].y - g->node[i].y) - L;
            }
            else
                dy = g->node[j].y - g->node[i].y;

            dist = sqrt(dx*dx + dy*dy);

            weight = f(alpha,dist);
            list->weight = weight;

            list = list->next;
        }
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

/*! \fn double calculate_energy(gs_graph_t *g)
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

/*! \fn double calculate_magnetisation(gs_graph_t *g)
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

/*! \fn void metropolis_monte_carlo_sweeps(gs_graph_t *g, int N)
    \brief Führt N Monte Carlo Sweeps durch mit dem Metropolis Algorithmus

    Dabei besteht ein Sweep aus \f$ L^2 \f$ Monte Carlo Schritten

    \param [in,out]    g    Graph, der modifiziert werden soll
    \param [in]        N    Anzahl der zu berechnenden Sweeps
*/
void metropolis_monte_carlo_sweeps(gs_graph_t *g, int N)
{
    int i;
    int num_nodes;
    int to_flip_idx;
    int delta_E;
    double A;
    elem_t *list;

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
        /*! - Berechne die Wahrscheinlichkeit, mit der der Flip akzeptiert
            wird.
            \f[ A = \left\{
                        \begin{array}{ll}
                            e^{-\beta \Delta E} & \Delta E > 0 \\
                            1                   & sonst
                        \end{array}
                    \right.  \f]
            Wie in \cite newman1999monte S. 49 (3.7) gegeben.
         */
        A = exp(-delta_E/g->T);
        if(A > my_rand())
        {
            /* drehe den Spin um */
            g->node[to_flip_idx].spin *= -1;

            g->E += delta_E;

            //~ g->M += 2 * g->node[to_flip_idx].spin;
        }
    }
}

/*! \fn void wolff_monte_carlo_sweeps(gs_graph_t *g, int N)
    \brief Führt N Monte Carlo Sweeps durch mit dem Wolff Cluster Algorithmus

    Dabei besteht ein Sweep aus einem Clusterschritt.\n
    Ablauf:
    - Cluster bilden
        - Einen zufälligen Seed Spin wählen
        - Nachbarn, die in die gleiche Richtung zeigen
            mit der Wahrscheinlichkeit \f$ P_add 1-e^{(-2J/T)} \f$
            hinzufügen
        - Nachbarn der Nachbarn auf gleiche Weise hinzufügen, bis es
            keine weitere Möglichkeit mehr gibt, Spins hinzuzufügen
    - Alle Spins in diesem Cluster flippen

    Vergleiche auch \cite newman1999monte (4.12)

    \param [in,out]    g    Graph, der modifiziert werden soll
    \param [in]        N    Anzahl der zu berechnenden Sweeps
*/
void wolff_monte_carlo_sweeps(gs_graph_t *g, int N)
{
    int i,n;                                     /* Counter Variablen */
    int cur_index;              /* Welchen Spin betrachte ich gerade? */
    elem_t *list;                    /* Temporär: finden der Nachbarn */
    int *cluster;     /* Liste der in den Cluster aufgenommenen Spins */
    double p_add;    /* Wahscheinlich einen weitern Spin hinzuzufügen */
    /* Stack mit Spins, deren Nachbarn noch die Chance bekommen müssen
     *                           in den Cluster aufgenommen zu werden */
    stack_t *stack_of_spins_with_untestet_neighbors = NULL;

    /* Allokation */
    cluster = malloc(g->num_nodes*sizeof(int));

    for(i=0;i<g->num_nodes*N;n++)
    {
        /* Initialisierung */
        for(i=0;i<g->num_nodes;i++)
            cluster[i]=0;

        /* Wähle zufälligen seed */
        cur_index = (int) (my_rand() * g->num_nodes);
        cluster[cur_index] = 1;
        stack_of_spins_with_untestet_neighbors
            = push(stack_of_spins_with_untestet_neighbors, cur_index);
        while(!is_empty(stack_of_spins_with_untestet_neighbors))
        {
            stack_of_spins_with_untestet_neighbors
                = pop(stack_of_spins_with_untestet_neighbors, &cur_index);
            /* Suche nach benachbarten Knoten mit gleichem Spin */
            list = g->node[cur_index].neighbors;
            while(list != NULL)
            {
                /* Wenn dieser Nachbar noch nicht im Cluster ist, und den
                 * gleichen Spin hat ... */
                if( (!cluster[list->index]) &&
                    (g->node[list->index].spin == g->node[cur_index].spin))
                {
                    /* ... gib ihm die Chance in den Cluster zu kommen */
                    /* Nicht vorher berechenbar, da J beliebig */
                    p_add = 1-exp(-2*list->weight/g->T);
                    if(p_add > my_rand())
                    {
                        cluster[list->index] = 1;
                        stack_of_spins_with_untestet_neighbors
                            = push(stack_of_spins_with_untestet_neighbors, list->index);
                    }
                }
                list = list->next;
            }
        }

        /* Jetzt flippe alle Spins im Cluster */
        for(i=0;i<g->num_nodes;i++)
            if(cluster[i])
                g->node[i].spin *= -1;

        /* Sollte eigentlich schon leer sein... */
        clear_stack(stack_of_spins_with_untestet_neighbors);
    }
    free(cluster);
}
