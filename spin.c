#include "spin.h"

/*! \fn int main(int argc, char *argv[])
    \brief Dies ist die Main Funktion.

    Sie kümmert sich um
    -# das Parsen von Kommandozeilenargumenten
    -# das Initialisieren des Graphens, auf dem die Spins liegen
    -# das Aufrufen der Funktionen, die die Monte Carlo Sweeps durchführen
        - die die Ergebnisse in einer Datei speichern

    \param [in] argc     Anzahl Kommandozeilen Argumente
    \param [in] argv     Vektor der Kommandozeilenargumente
*/
int main(int argc, char *argv[])
{
    gs_graph_t **list_of_graphs;
    int nT;

    /* Vars für getopt (Kommandozeilenparser) */
    options_t o;

    o = get_cl_args(argc, argv);

    list_of_graphs = init_graphs(o);

    do_mc_simulation(list_of_graphs, o);

    /* gebe Speicher frei */
    for(nT=0;nT<o.num_temps;nT++)
        gs_clear_graph(list_of_graphs[nT]);
    free(list_of_graphs);
    free(o.list_of_temps);
    free_my_rand();

    return(0);
}

/*! \fn options_t get_cl_args(int argc, char *argv[])
    \brief Diese Funktion interpretiert die Kommandozeilenargumente mithilfe von
            getopt und gibt sie wieder zurück. Und vergibt für den Rest
            Standardwerte.

    \param [in]  argc           Anzahl der Argumente
    \param [in]  argv           Vektor der Argumente
    \return struct mit allen wichtigen Informationen
*/
options_t get_cl_args(int argc, char *argv[])
{
    int c;
    int i, j, nT;
    int seed;
    int custom_file_name;
    int wolff_flag;
    char temp_string[20];
    extern char *optarg;

    options_t o;

    /* Standardwerte, wenn keine Optionen gegeben */
    /* Temperatur */
    o.list_of_temps = NULL;
    /* Kantenlänge des Feldes */
    o.L=64;
    /* Wieviele Sweeps berechnen */
    /* Ein Sweep sind \f$ L^2 \f$ Monte Carlo Schritte */
    o.N=2000;
    /* nach wie vielen Sweeps ist das Equilibrium erreicht (vgl. t_eq.dat) */
    o.t_eq = 1000;
    /* Parameter, der die Verschiebung der einzelnen Knoten bestimmt */
    o.moving_fkt = gauss;
    o.sigma = 0;
    /* Parameter, der die Gewichtung der Kanten bestimmt */
    o.weighting_fkt = exponential_decay;
    o.alpha = 0;
    /* Anfangsbedingung der Spins: 0: zufällig, 1: alle up */
    o.start_order = 0;
    /* Seed für Zufallsgenerator */
    seed = 42;
    /* Soll Wolff Algorithmus benutzt werden? */
    wolff_flag = 0;
    /* Soll Parallel Tempering Algorithmus benutzt werden? */
    o.par_temp_flag = 0;
    o.num_temps = 1;
    /* weitere Optionen */
    o.verbose = 0;
    custom_file_name = 0;


    opterr = 0;
    while ((c = getopt (argc, argv, "hvT:L:x:N:e:s:o:u:i:wp")) != -1)
        switch (c)
        {
            case 'T':
                i=0;
                /* Ermittele Anzahl der Temperaturen */
                while(optarg[i]!= '\0')
                    if(optarg[i++] == ',')
                        (o.num_temps)++;
                /* Reserviere Speicher */
                o.list_of_temps = (double*) malloc(o.num_temps * sizeof(double));
                /* Schreibe die Temperaturen als Double in das Array */
                nT=0; j=0; i=0;
                do
                {
                    if(optarg[i] == ',' || optarg[i] == '\0')
                    {
                        temp_string[j] = '\0';
                        j=0;
                        (o.list_of_temps)[nT++] = atof(temp_string);
                    }
                    else
                    {
                        temp_string[j] = optarg[i];
                        j++;
                    }
                } while(optarg[i++]!= '\0');
                break;
            case 'L':
                o.L = atoi(optarg);
                break;
            case 'x':
                seed = atoi(optarg);
                break;
            case 'N':
                o.N = atoi(optarg);
                break;
            case 'e':
                o.t_eq = atoi(optarg);
                break;
            case 's':
                o.sigma = atof(optarg);
                break;
            case 'o':
                custom_file_name = 1;
                strncpy(o.filename, optarg, MAX_LEN_FILENAME);
                break;
            case 'u':
                o.start_order = atoi(optarg);
                break;
            case 'v':
                o.verbose = 1;
                break;
            case 'w':
                wolff_flag = 1;
                break;
            case 'p':
                o.par_temp_flag = 1;
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
                fprintf(stderr,"    -w     Wolff Algorithmus (statt Metropolis)      \n");
                fprintf(stderr,"    -px    Parallel Tempering mit einer Komma        \n");
                fprintf(stderr,"            getrennten Liste der zu berechnenden Temperaturen \n");
                exit(1);
            default:
                abort ();
        }

    if(! o.list_of_temps)
    {
        o.list_of_temps = (double*) malloc(o.num_temps * sizeof(double));
        o.list_of_temps[0] = 2.0;
    }
    if(!custom_file_name)
    {
        /* standard Dateiname */
        snprintf(o.filename, MAX_LEN_FILENAME, "data/data_L_%d.dat", o.L);
    }

    /* Welchen Algorithmus nutzen? */
    if(! wolff_flag)
        o.mc_fkt = &metropolis_monte_carlo_sweeps;
    else
        o.mc_fkt = &wolff_monte_carlo_sweeps;

    smy_rand(seed);

    if(o.verbose)
    {
        printf("gewählte Parameter:\n");
        printf("    L     = %d\n", o.L);
        printf("    T     =  \n");
        for(i=0;i<o.num_temps;i++)
            printf("            %f,\n",(o.list_of_temps)[i]);
        printf("           \n");
        printf("    N     = %d\n", o.N);
        printf("    t_eq  = %d\n", o.t_eq);
        printf("    seed  = %d\n", seed);
        printf("    sigma = %f\n", o.sigma);
        if(o.start_order)
            printf("    spins starten alle up\n");
        else
            printf("    spins starten zufällig\n");
        if(wolff_flag)
            printf("    Wolff Algorithmus\n");
        else
            printf("    Metropolis Algorithmus\n");
        if(o.par_temp_flag)
            printf("    Parallel Tempering aktiviert\n");
        else
            printf("    Parallel Tempering deaktiviert\n");
        printf("    Filename: '%s'\n",o.filename);
        printf("    \n");
    }

    return(o);
}

/*! \fn gs_graph_t **init_graphs(options_t o)
    \brief Diese Funktion initialisiert die Graphen mit allen nötigen
            Informationen

    \param [in] o               struct mit allen wichtigen Informationen
    \return Array mit den initialisierten Graphen
*/
gs_graph_t **init_graphs(options_t o)
{
    gs_graph_t *g;
    gs_graph_t **list_of_graphs;

    int nT;

    list_of_graphs = (gs_graph_t**) malloc(o.num_temps * sizeof(gs_graph_t*));

    /* Initialisiere den Graphen für die Spins */
    g = gs_create_graph(o.L);

    /* Verschiebe die Knoten */
    move_graph_nodes(g, o.moving_fkt, o.sigma);

    /* Verknüpfe die Knoten */
    create_edges_regular(g);
    assign_weights_with_function(g, o.weighting_fkt, o.alpha);

    for(nT=0;nT<o.num_temps;nT++)
    {
        /* initialisiere den Status der Spins */
        if(o.start_order == 1)
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
        list_of_graphs[nT]->T = o.list_of_temps[nT];
    }
    gs_clear_graph(g);

    return(list_of_graphs);
}

/*! \fn void do_mc_simulation(gs_graph_t **list_of_graphs, options_t o)
    \brief Diese Funktion führt die MC Berechnungen durch und speichert die
            Ergebnisse

    \param [in] list_of_graphs  Liste der Graphen, die für parallel Tempering
                                genutzt werden
    \param [in] o               struct mit allen wichtigen Informationen
*/
void do_mc_simulation(gs_graph_t **list_of_graphs, options_t o)
{
    double *par_temp_versuche, *par_temp_erfolge;
    FILE *data_out_file;
    int nT;
    int bool_par_temp_already_running;
    int *map_of_temps;

    /* Multithreading */
    /* Mit Barrier (deshalb _XOPEN_SOURCE 600)
     * Alle Threads warten an der Barrierer aufeinander, dann werden
     * Temperaturen ausgetauscht, solange warten die Threads hinter der
     * Barrier. Dann geht es weiter. */
    pthread_t *calc_temps;
    pthread_mutex_t par_temp_mutex;
    pthread_barrier_t barr;

    calc_temps = (pthread_t*) malloc(o.num_temps * sizeof(pthread_t));
    pthread_mutex_init(&par_temp_mutex, NULL);
    pthread_barrier_init(&barr, NULL, o.num_temps);
    bool_par_temp_already_running = 0;

    /* Am Anfang sollten die Temperaturen sortiert sein */
    map_of_temps = (int*) malloc(o.num_temps * sizeof(int));
    for(nT=0;nT<o.num_temps;nT++)
        map_of_temps[nT] = nT;

    /* Allokation und Initialisierung zur Statistik der Parallel Tempering Übergänge */
    par_temp_versuche = (double*) calloc(o.num_temps, sizeof(double));
    par_temp_erfolge  = (double*) calloc(o.num_temps, sizeof(double));

    /* Führe Monte Carlo Sweeps durch */
    /* Erreiche das Equilibrium */
    /* Schreibe alle inc Sweeps die Energie und Magnetisierung in eine Datei */
    /* Plotte den Ausdruck mit Gnuplot. zB.
     * plot 'test.dat' using 1:2, "test.dat" using 1:3; */
    data_out_file = fopen(o.filename, "w");
    if(data_out_file == NULL)
    {
        fprintf(stderr,"ERROR: %s kann nicht geöffnet werden",o.filename);
        exit(-1);
    }
    /* Schreibe Header */
    fprintf(data_out_file, "# N E M # L=%d # T= ", list_of_graphs[0]->L);
    for(nT=0;nT<o.num_temps;nT++)
        fprintf(data_out_file, "%.3f, ", list_of_graphs[nT]->T);
    fprintf(data_out_file, "\n");

    void write_data_to_file_wrap_for_threads(int N)
    {
        write_data_to_file(data_out_file,list_of_graphs,map_of_temps,o,N);
    }

    void par_temp_wrap_for_threads()
    {
        par_temp(list_of_graphs, map_of_temps, o,
                                    par_temp_versuche,par_temp_erfolge);
    }

    void *sweep(void *t)
    {
        int i;
        gs_graph_t* g;
        g = (gs_graph_t*) t;
        /* Führe N Sweeps durch */
        for(i=0;i<o.N;i++)
        {
            /* inc MC Sweeps durchführen */
            o.mc_fkt(g);

            g->M = calculate_magnetisation(g);

            pthread_barrier_wait(&barr);

            pthread_mutex_lock(&par_temp_mutex);
            if(!bool_par_temp_already_running)
            {
                write_data_to_file_wrap_for_threads(i);

                bool_par_temp_already_running = 1;
                par_temp_wrap_for_threads();
            }
            pthread_mutex_unlock(&par_temp_mutex);

            pthread_barrier_wait(&barr);
            bool_par_temp_already_running = 0;
        }
        return(NULL);
    }

    for(nT=0;nT<o.num_temps;nT++)
        pthread_create(&calc_temps[nT], NULL, &sweep, (void *) list_of_graphs[nT]);
    for(nT=0;nT<o.num_temps;nT++)
        pthread_join(calc_temps[nT], NULL);

    if(o.verbose)
    {
        fprintf(stderr,"Akzeptanzniveaus: \n");
        fprintf(stderr,"T: ");
        for(nT=0;nT<o.num_temps;nT++)
            fprintf(stderr,"%.2f      ",list_of_graphs[map_of_temps[nT]]->T);
        fprintf(stderr,"\n");
        fprintf(stderr,"A: ");
        for(nT=0;nT<o.num_temps;nT++)
            fprintf(stderr,"     %.2f ", par_temp_erfolge[map_of_temps[nT]]/par_temp_versuche[map_of_temps[nT]]);
        fprintf(stderr,"\n");
    }

    fclose(data_out_file);
    free(par_temp_versuche);
    free(par_temp_erfolge);
    pthread_mutex_destroy(&par_temp_mutex);
    free(calc_temps);
    free(map_of_temps);
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

/*! \fn void create_edges_relative_neighborhood(gs_graph_t *g)
    \brief Fügt Kanten zu einem Graphen hinzu, wodurch ein
            Relative Neighborhood Graph erzeugt werden soll.

            Kantengewichte (J) werden durch eine übergebene Funktion aus
            dem (euklidischen) Abstand der Knoten ermittelt

    \param [in,out]    g    Graph, der modifiziert werden soll
*/
void create_edges_relative_neighborhood(gs_graph_t *g)
{
    /* Zuerst kachele einen Graphen, aus neun Kopien des Originalgraphen,
        um die periodischen Randbedingen zu beachten.
        Erstelle den Graphen und berechne aus den Abständen, die dabei
        berechnet werden können die Kantengewichte.
        Danach lösche die Kopien. */
}

/*! \fn void assign_weights_with_function(gs_graph_t *g,
                   double (*f)(double alpha, double dist), double alpha)
    \brief Ändert die Kantengewichte eines Graphen abhängig vom Abstand
            der Knoten nach einem exponentiellen Zusammenhang.

            Dieser Exponentielle Zusammenhang ist
            \f$ J = e^{-\alpha d} \f$
            wobei d der Abstand er Knoten ist

    \param [in,out]    g     Graph, der modifiziert werden soll
    \param [in]        f     Gewichtungsfunktion
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

/*! \fn void metropolis_monte_carlo_sweeps(gs_graph_t *g)
    \brief Führt N Monte Carlo Sweeps durch mit dem Metropolis Algorithmus

    Dabei besteht ein Sweep aus \f$ L^2 \f$ Monte Carlo Schritten

    \param [in,out]    g    Graph, der modifiziert werden soll
*/
void metropolis_monte_carlo_sweeps(gs_graph_t *g)
{
    int i;
    int num_nodes;
    int to_flip_idx;
    int delta_E;
    double A;
    elem_t *list;

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes;i++)
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
        }
    }
}

/*! \fn void wolff_monte_carlo_sweeps(gs_graph_t *g)
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
*/
void wolff_monte_carlo_sweeps(gs_graph_t *g)
{
    int i,n;                                     /* Counter Variablen */
    int cur_index;              /* Welchen Spin betrachte ich gerade? */
    elem_t *list;                    /* Temporär: finden der Nachbarn */
    int *cluster;     /* Liste der in den Cluster aufgenommenen Spins */
    double p_add;    /* Wahscheinlich einen weitern Spin hinzuzufügen */
    /* Stack mit Spins, deren Nachbarn noch die Chance bekommen müssen
     *                           in den Cluster aufgenommen zu werden */
    stack_t stack_of_spins_with_untestet_neighbors;

    /* Allokation */
    stack_of_spins_with_untestet_neighbors = create_stack(g->num_nodes);
    cluster = malloc(g->num_nodes*sizeof(int));

    //~ for(n=0;n<g->num_nodes;n++)
    for(n=0;n<1;n++)
    {
        /* Initialisierung */
        for(i=0;i<g->num_nodes;i++)
            cluster[i]=0;

        /* Wähle zufälligen seed */
        cur_index = (int) (my_rand() * g->num_nodes);
        cluster[cur_index] = 1;
        push(&stack_of_spins_with_untestet_neighbors, cur_index);
        while(!is_empty(&stack_of_spins_with_untestet_neighbors))
        {
            cur_index = pop(&stack_of_spins_with_untestet_neighbors);
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
                        push(&stack_of_spins_with_untestet_neighbors, list->index);
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
        clear_stack(&stack_of_spins_with_untestet_neighbors);
    }
    g->E = calculate_energy(g);
    free(cluster);
}

/*! \fn void par_temp(gs_graph_t **list_of_graphs, int *map_of_temps,
             options_t o, double *par_temp_versuche, double *par_temp_erfolge)
    \brief Für den tausch der Temperaturen für parallel Tempering aus


    Vergleiche auch \cite newman1999monte (6.4)

    \param [in,out] list_of_graphs  Graphen, deren Temperaturen
                                    ausgetauscht werden
    \param [in,out] map_of_temps    Liste der Indizes der Graphen aus
                    list_of_graphs aufsteigend nach Temperatur sortiert
    \param [in]     o               allgemeine Informationen
    \param [in,out] par_temp_versuche   für statistische Zwecke
    \param [in,out] par_temp_erfolge   für statistische Zwecke
*/
void par_temp(gs_graph_t **list_of_graphs, int *map_of_temps,
             options_t o, double *par_temp_versuche, double *par_temp_erfolge)
{
    int j;
    int temp_index, temp_index_p;
    double delta;
    double tmp_T;

    /* Parallel Tempering Austausch */
    if(o.par_temp_flag)
    {
        //~ temp_index = (int) (my_rand() * (num_temps-1));
        for(j=0;j<o.num_temps-1;j++)
        {
            temp_index = map_of_temps[j];
            temp_index_p = map_of_temps[j+1];

            par_temp_versuche[temp_index]++;
            /* Wähle zufällig, ob getauscht werden soll */
            delta = ( 1/list_of_graphs[temp_index]->T - 1/list_of_graphs[temp_index_p]->T )
                    * ( list_of_graphs[temp_index]->E - list_of_graphs[temp_index_p]->E);
            if(my_rand() < exp(delta))
            {
                par_temp_erfolge[temp_index]++;
                /* Tausche die Temperaturen ... */
                tmp_T = list_of_graphs[temp_index]->T;
                list_of_graphs[temp_index]->T = list_of_graphs[temp_index_p]->T;
                list_of_graphs[temp_index_p]->T = tmp_T;
                /* ... und führe Buch */
                map_of_temps[j] = temp_index_p;
                map_of_temps[j+1] = temp_index;
            }
        }
    }
}

/*! \fn void write_data_to_file(FILE *data_out_file, gs_graph_t **list_of_graphs,
                                int *map_of_temps, options_t o, int N)
    \brief Schreibt eine Zeile (also einen Sweep) in die Ausgabedatei

    \param [in]     data_out_file   Datei, in die geschrieben wird
    \param [in,out] list_of_graphs  Graphen, die deren Kennziffern
                                    geschrieben werden sollen
    \param [in,out] map_of_temps    Liste der Indizes der Graphen aus
                    list_of_graphs aufsteigend nach Temperatur sortiert
    \param [in]     o               allgemeine Informationen
    \param [in,out] N               Nummer des aktuellen Sweeps
*/
void write_data_to_file(FILE *data_out_file, gs_graph_t **list_of_graphs,
                                int *map_of_temps, options_t o, int N)
{
    int j;
    int nT;
    /* Schreibe in Datei */
    if(N > o.t_eq)
    {
        fprintf(data_out_file, "%d ", N);
        for(nT=0;nT<o.num_temps;nT++)
        {
            /* An welcher Stelle liegt die nT-te Temperatur? */
            j = map_of_temps[nT];
            fprintf(data_out_file, "%f %f ", list_of_graphs[j]->E,
                 list_of_graphs[j]->M/list_of_graphs[j]->num_nodes);
        }
        fprintf(data_out_file, "\n");
    }
}
