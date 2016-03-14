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

    if(o.verbose)
        fprintf(stdout,"Starte Initialisierung\n");
    list_of_graphs = init_graphs(o);

    if(o.verbose)
        fprintf(stdout,"Starte Monte Carlo Simulation\n");
    do_mc_simulation(list_of_graphs, o);

    if(o.svg_filename[0] != '\0')
        print_graph_svg(list_of_graphs[0], o.svg_filename);

    /* gebe Speicher frei */
    for(nT=1;nT<o.num_temps;nT++)
        gs_clear_shallow_graph(list_of_graphs[nT]);
    gs_clear_graph(list_of_graphs[0]);
    free(list_of_graphs);
    free(o.list_of_temps);
    gsl_rng_free(o.rng);

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
    /* alle wieviel sweeps soll ein Wert gespeichert werden (tau) */
    o.tau = 1;
    /* Parameter, der den Graphentyp beschreibt */
    o.graph_type = 1;
    /* Parameter, der die Verschiebung der einzelnen Knoten bestimmt */
    o.moving_fkt = &gsl_ran_gaussian;
    o.sigma = 0;
    /* Parameter, der die Gewichtung der Kanten bestimmt */
    o.weighting_fkt = &exponential_decay;
    o.alpha = 0;
    /* Anfangsbedingung der Spins: 0: zufällig, 1: alle up */
    o.start_order = 0;
    /* Seed für Zufallsgenerator */
    o.seed = 42;
    /* soll ein Percolationsübergang erzeugt werden? */
    o.percolation = 0;
    /* Soll Wolff Algorithmus benutzt werden? */
    wolff_flag = 0;
    /* Soll Parallel Tempering Algorithmus benutzt werden? */
    o.par_temp_flag = 0;
    o.num_temps = 1;
    /* weitere Optionen */
    o.verbose = 0;
    o.zip = 0;
    custom_file_name = 0;
    /* SVG Output */
    o.svg_filename[0]='\0';

    opterr = 0;
    while ((c = getopt (argc, argv, "hvT:L:x:N:e:s:a:o:g:u:i:wpt:zr")) != -1)
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
                o.seed = atoi(optarg);
                break;
            case 'N':
                o.N = atoi(optarg);
                break;
            case 'e':
                o.t_eq = atoi(optarg);
                break;
            case 'i':
                o.tau = atoi(optarg);
                break;
            case 's':
                o.sigma = atof(optarg);
                break;
            case 'a':
                o.alpha = atof(optarg);
                break;
            case 't':
                o.graph_type = atoi(optarg);
                break;
            case 'o':
                custom_file_name = 1;
                strncpy(o.filename, optarg, MAX_LEN_FILENAME);
                break;
            case 'g':
                strncpy(o.svg_filename, optarg, MAX_LEN_FILENAME);
                break;
            case 'u':
                o.start_order = atoi(optarg);
                break;
            case 'v':
                o.verbose = 1;
                break;
            case 'z':
                o.zip = 1;
                break;
            case 'r':
                o.percolation = 1;
                break;
            case 'w':
                wolff_flag = 1;
                break;
            case 'p':
                o.par_temp_flag = 1;
                break;
            case '?':
                fprintf(stdout,
                        "Unknown option character `\\x%x'.\n", optopt);
            case 'h':
                fprintf(stdout,"Benutzung: %s -[hTLxNesou]\n",argv[0]);
                fprintf(stdout,"    -h     Zeigt diese Hilfe                         \n");
                fprintf(stdout,"    -v     gesprächiger Modus                        \n");
                fprintf(stdout,"    -Tx    Temperatur x                      (double)\n");
                fprintf(stdout,"    -Lx    Länge x                              (int)\n");
                fprintf(stdout,"    -xx    seed x                               (int)\n");
                fprintf(stdout,"    -Nx    x Monte Carlo sweeps                 (int)\n");
                fprintf(stdout,"    -ex    Equilibrium nach x sweeps angenommen (int)\n");
                fprintf(stdout,"    -ix    autokorrelationszeit                 (int)\n");
                fprintf(stdout,"    -sx    sigma x                           (double)\n");
                fprintf(stdout,"    -ax    alpha x                           (double)\n");
                fprintf(stdout,"    -tx    Graphtyp: (1: RNG, 2: Gabriel, 3: MST)(int)\n");
                fprintf(stdout,"    -ox    filename (max. 79 Zeichen)        (string)\n");
                fprintf(stdout,"    -gx    SVG Filename (max. 79 Zeichen)    (string)\n");
                fprintf(stdout,"    -ux    Ordnung x (0: zufällig, 1: alle up)  (int)\n");
                fprintf(stdout,"    -w     Wolff Algorithmus (statt Metropolis)      \n");
                fprintf(stdout,"    -p     Parallel Tempering                        \n");
                fprintf(stdout,"    -r     Percolations Übergang                     \n");
                fprintf(stdout,"    -z     gzip outputfile                           \n");
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
        snprintf(o.filename, MAX_LEN_FILENAME, "data/data_s_%.3f_x_%03d_L_%03d_type_%d_r%d.dat", o.sigma, o.seed, o.L, o.graph_type, o.percolation);
    }

    /* Welchen Algorithmus nutzen? */
    if(! wolff_flag)
        /* Kein Wolff -> Metropolis */
        o.mc_fkt = &metropolis_monte_carlo_sweep;
    else if(o.par_temp_flag)
        /* Wolff und Parallel Tempering
         * -> führe vorher einen Metropolis Sweep aus */
        o.mc_fkt = &metroplis_sweep_and_wolff_cluster;
    else
        /* Wolff ohne Parallel Tempering
         * -> führe nur Wolff aus */
        o.mc_fkt = &wolff_monte_carlo_cluster;

    /* Welchen Graphen bauen? */
    if(o.graph_type == 1)
    {
        o.graph_fkt = &check_relative_neighborhood;
        o.graph_cell_border_fkt = &get_cell_border_relative_neighborhood;
    }
    else
    {
        o.graph_fkt = &check_gabriel;
        o.graph_cell_border_fkt = &get_cell_border_gabriel;
    }

    /* Setup RNG */
    gsl_rng_env_setup();
    o.rng = gsl_rng_alloc (gsl_rng_mt19937);
    gsl_rng_set(o.rng, o.seed);

    if(o.verbose)
    {
        printf("gewählte Parameter:\n");
        printf("    L     = %d\n", o.L);
        printf("    T     = ");
        for(i=0;i<o.num_temps;i++)
            printf("%.2f, ",(o.list_of_temps)[i]);
        printf("\n");
        printf("           \n");
        printf("    N     = %d\n", o.N);
        printf("    t_eq  = %d\n", o.t_eq);
        printf("    tau   = %d\n", o.tau);
        printf("    seed  = %d\n", o.seed);
        printf("    sigma = %f\n", o.sigma);
        printf("    alpha = %f\n", o.alpha);
        if(o.start_order)
            printf("    spins starten alle up\n");
        else
            printf("    spins starten zufällig\n");

        if(wolff_flag)
            printf("    Wolff Algorithmus\n");
        else
            printf("    Metropolis Algorithmus\n");

        if(o.graph_type == 1)
            printf("    Relative Neighborhood Graph\n");
        else
            printf("    Gabriel Graph\n");

        if(o.par_temp_flag)
            printf("    Parallel Tempering aktiviert\n");
        else
            printf("    Parallel Tempering deaktiviert\n");

        if(o.percolation)
            printf("    Percolation aktiviert\n");

        if(o.svg_filename[0] != '\0')
            printf("    Graph SVG: '%s'\n", o.svg_filename);

        printf("    Filename: '%s'\n",o.filename);
        printf("    \n");
    }

    return(o);
}

/*! \fn void do_mc_simulation(gs_graph_t **list_of_graphs, options_t o)
    \brief Diese Funktion führt die MC Berechnungen durch und speichert die
            Ergebnisse

    \param [in] list_of_graphs  Liste der Graphen, die für parallel Tempering
                                genutzt werden
    \param [in] o               struct mit allen wichtigen Informationen
*/
void do_mc_simulation(gs_graph_t **list_of_graphs, const options_t o)
{
    double *par_temp_versuche, *par_temp_erfolge;
    FILE *data_out_file;
    int nT;
    int j;
    int *map_of_temps;
    char command[MAX_LEN_FILENAME+10];

    #ifdef THREADED
        /* Multithreading */
        /* Mit Barrier (deshalb _XOPEN_SOURCE 600)
         * Alle Threads warten an der Barrierer aufeinander, dann werden
         * Temperaturen ausgetauscht, solange warten die Threads hinter der
         * Barrier. Dann geht es weiter. */
        int bool_par_temp_already_running;
        pthread_t *calc_temps;
        pthread_mutex_t par_temp_mutex;
        pthread_barrier_t barr;

        calc_temps = (pthread_t*) malloc(o.num_temps * sizeof(pthread_t));
        pthread_mutex_init(&par_temp_mutex, NULL);
        pthread_barrier_init(&barr, NULL, o.num_temps);
        bool_par_temp_already_running = 0;
    #else
        int i;
    #endif

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
    fprintf(data_out_file, "# N E M chi_0 chi_k # sigma=%.3f # x=%d # L=%d # type=%d # r=%d # <J>=%.6f # d=%.6f # deg=%.6f # sumJ=%.6f # T= ", o.sigma, o.seed, list_of_graphs[0]->L, o.graph_type, o.percolation, get_mean_weight(list_of_graphs[0]), get_mean_dist(list_of_graphs[0]), get_mean_deg(list_of_graphs[0]), get_sum_weight(list_of_graphs[0]));
    for(nT=0;nT<o.num_temps;nT++)
        fprintf(data_out_file, "%.3f, ", list_of_graphs[nT]->T);
    fprintf(data_out_file, "\n");

    #ifdef THREADED
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
            gsl_rng * rng;

            g = (gs_graph_t*) t;
            rng = gsl_rng_alloc (gsl_rng_mt19937);
            gsl_rng_set(rng, 42+nT+13);

            /* Führe N Sweeps durch */
            for(i=0;i<o.N;i++)
            {
                o.mc_fkt(g, rng);

                g->M = calculate_magnetisation(g);

                pthread_barrier_wait(&barr);

                pthread_mutex_lock(&par_temp_mutex);
                if(!bool_par_temp_already_running)
                {
                    write_data_to_file_wrap_for_threads(i);

                    bool_par_temp_already_running = 1;
                    if(i==o.t_eq)
                        for(j=0;j<o.num_temps-1;j++)
                        {
                            par_temp_versuche[j]=0;
                            par_temp_erfolge[j]=0;
                        }
                    par_temp_wrap_for_threads();
                }
                pthread_mutex_unlock(&par_temp_mutex);

                pthread_barrier_wait(&barr);
                bool_par_temp_already_running = 0;
            }
            gsl_rng_free(rng);
            return(NULL);
        }

        for(nT=0;nT<o.num_temps;nT++)
            pthread_create(&calc_temps[nT], NULL, &sweep, (void *) list_of_graphs[nT]);
        for(nT=0;nT<o.num_temps;nT++)
            pthread_join(calc_temps[nT], NULL);
    #else
        for(i=0;i<o.N;i++)
        {
            for(nT=0;nT<o.num_temps;nT++)
            {
                o.mc_fkt(list_of_graphs[nT], o.rng);

                list_of_graphs[nT]->M = calculate_magnetisation(list_of_graphs[nT]);
            }
            write_data_to_file(data_out_file,list_of_graphs,map_of_temps,o,i);

            /* Fange erst mit dem Zählen an, wenn du im Equilibrium bist */
            if(i==o.t_eq)
                for(j=0;j<o.num_temps-1;j++)
                {
                    par_temp_versuche[j]=0;
                    par_temp_erfolge[j]=0;
                }
            par_temp(list_of_graphs, map_of_temps, o, par_temp_versuche,par_temp_erfolge);
        }
    #endif

    if(o.verbose)
    {
        fprintf(stdout,"Akzeptanzniveaus: \n");
        fprintf(stdout,"T: ");
        for(nT=0;nT<o.num_temps;nT++)
            fprintf(stdout,"%.2f      ",list_of_graphs[map_of_temps[nT]]->T);
        fprintf(stdout,"\n");
        fprintf(stdout,"A: ");
        for(nT=0;nT<o.num_temps;nT++)
            fprintf(stdout,"     %.2f ", par_temp_erfolge[map_of_temps[nT]]/par_temp_versuche[map_of_temps[nT]]);
        fprintf(stdout,"\n");
        fprintf(data_out_file,"# A: ");
        for(nT=0;nT<o.num_temps;nT++)
            fprintf(data_out_file,"%.2f ", par_temp_erfolge[map_of_temps[nT]]/par_temp_versuche[map_of_temps[nT]]);
        fprintf(data_out_file,"\n");
    }

    fclose(data_out_file);
    if(o.zip)
    {
        sprintf(command, "gzip %s", o.filename);
        system(command);                           /* gzip Outputfile */
    }

    free(par_temp_versuche);
    free(par_temp_erfolge);
    #ifdef THREADED
        pthread_mutex_destroy(&par_temp_mutex);
        free(calc_temps);
    #endif
    free(map_of_temps);
}

/*! \fn double exponential_decay(double alpha, double x)
    \brief Berechnet den exponentiellen Abfall \f$ e^{-\alpha x} \f$

    \param [in]   alpha    Parameter des Abfalls
    \param [in]   x        Argument der Funktion
    \return \f$ e^{-\alpha x} \f$
*/
double exponential_decay(const double alpha, const double x)
{
    return(exp(-alpha*(x-1)));
}

/*! \fn double calculate_energy(const gs_graph_t *g)
    \brief Berechnet die Energie des Ising Modells

    Dies wird durch einfache Auswertung des Hamiltonoperators des
    Ising Modells erledigt. \f[ E = H = -\sum_{<ij>} J_{ij} s_i s_j \f]
    Dabei ist J die Kopplungskonstante, hier also das Kantengewicht.

    \param [in,out]    g    Graph, der modifiziert werden soll
    \return Energie des Systems.
*/
double calculate_energy(const gs_graph_t *g)
{
    int num_nodes, i;
    int n;
    int sk;                  /*< Wert des k-ten Spins (temporäre Var) */
    double E=0;                               /*< Energie des Systems */
    double E_sk;          /*< Energie des k-ten Spins (temporäre Var) */
    gs_edge_t *list;

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes;i++)                      /* Über alle Knoten */
    {
        E_sk = 0;
        sk = g->spins[i];
        list = g->node[i].neighbors;
        for(n=0;n<g->node[i].num_neighbors;n++) /* Über alle Nachbarn */
            E_sk += g->spins[list[n].index] * list[n].weight;
        E_sk *= sk;
        E += E_sk;
    }
    return(-E/2);   /* Da über alle Spins zweimal summiert wurde: E/2 */
}

/*! \fn double calculate_magnetisation(const gs_graph_t *g)
    \brief Berechnet die Magnetisierung des Ising Modells

    Dies wird durch einfache Auswertung der Definition erledigt.
    \f[ M = \sum_{i} s_i \f]

    \param [in,out]    g    Graph, der modifiziert werden soll
    \return Magnetisierung des Systems.
*/
double calculate_magnetisation(const gs_graph_t *g)
{
    int num_nodes, i;
    double M=0;

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes;i++)                      /* Über alle Knoten */
        M += g->spins[i];

    return(M);
}

/*! \fn void metropolis_monte_carlo_sweep(gs_graph_t *g, gsl_rng *rng)
    \brief Führt N Monte Carlo Sweeps durch mit dem Metropolis Algorithmus

    Dabei besteht ein Sweep aus \f$ L^2 \f$ Monte Carlo Schritten

    \param [in,out]    g    Graph, der modifiziert werden soll
    \param [in]      rng    Zufallszahlengenerator
*/
void metropolis_monte_carlo_sweep(gs_graph_t *g, gsl_rng *rng)
{
    int i;
    int n;
    int num_nodes;
    int to_flip_idx;
    double delta_E;
    double A;
    gs_edge_t *list;

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes;i++)
    {
        delta_E = 0;
        /*! - Ermittele den zu flippenden Spin zufällig. */
        to_flip_idx = floor(gsl_rng_uniform(rng)*num_nodes);
        /*! - Ermittele Energieänderung
            \f[ \Delta E = 2s_k \sum_{\mathrm{adj}(s_k)} J_{kj} s_j \f]
            So wie in \cite newman1999monte S. 52 (3.10) gegeben,
            Jedoch leicht abgewandelt, um verschiedene J zu
            berücksichtigen.
         */
        list = g->node[to_flip_idx].neighbors;
        for(n=0;n<g->node[to_flip_idx].num_neighbors;n++)
        {
            /* Hier wird die Summe ausgeführt: E += s_j * J_{ij} */
            delta_E += g->spins[list[n].index] * list[n].weight;
        }
        /* Hier werden die Koeffizienten berücksichtigt: E = 2s_k*sum */
        delta_E *= 2 * g->spins[to_flip_idx];
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
        if(A > gsl_rng_uniform(rng))
        {
            /* drehe den Spin um */
            g->spins[to_flip_idx] *= -1;

            g->E += delta_E;
        }
    }
}

/*! \fn void wolff_monte_carlo_cluster(gs_graph_t *g, gsl_rng *rng)
    \brief Flippt einen Wolff Cluster

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
    \param [in]      rng    Zufallszahlengenerator
*/
void wolff_monte_carlo_cluster(gs_graph_t *g, gsl_rng *rng)
{
    int i,j;                                   /* Counter Variablen */
    int cur_index;              /* Welchen Spin betrachte ich gerade? */
    gs_edge_t *list;                 /* Temporär: finden der Nachbarn */
    int *cluster;     /* Liste der in den Cluster aufgenommenen Spins */
    double p_add;    /* Wahscheinlich einen weitern Spin hinzuzufügen */
    /* Stack mit Spins, deren Nachbarn noch die Chance bekommen müssen
     *                           in den Cluster aufgenommen zu werden */
    stack_t stack_of_spins_with_untestet_neighbors;

    /* Allokation */
    stack_of_spins_with_untestet_neighbors = create_stack(g->num_nodes);
    cluster = malloc(g->num_nodes*sizeof(int));

    /* Initialisierung */
    for(i=0;i<g->num_nodes;i++)
        cluster[i]=0;

    /* Wähle zufälligen seed */
    cur_index = (int) (gsl_rng_uniform(rng) * g->num_nodes);
    cluster[cur_index] = 1;
    push(&stack_of_spins_with_untestet_neighbors, cur_index);
    while(!is_empty(&stack_of_spins_with_untestet_neighbors))
    {
        cur_index = pop(&stack_of_spins_with_untestet_neighbors);
        /* Suche nach benachbarten Knoten mit gleichem Spin */
        list = g->node[cur_index].neighbors;
        for(j=0;j<g->node[cur_index].num_neighbors;j++)
        {
            /* Wenn dieser Nachbar noch nicht im Cluster ist, und den
             * gleichen Spin hat ... */
            if( (!cluster[list[j].index]) &&
                (g->spins[list[j].index] == g->spins[cur_index]))
            {
                /* ... gib ihm die Chance in den Cluster zu kommen */
                /* Nicht vorher berechenbar, da J beliebig */
                p_add = 1-exp(-2*list[j].weight/g->T);
                if(p_add > gsl_rng_uniform(rng))
                {
                    cluster[list[j].index] = 1;
                    push(&stack_of_spins_with_untestet_neighbors, list[j].index);
                }
            }
        }
    }

    /* Jetzt flippe alle Spins im Cluster */
    for(i=0;i<g->num_nodes;i++)
        if(cluster[i])
            g->spins[i] *= -1;

    g->E = calculate_energy(g);
    free(cluster);
    clear_stack(&stack_of_spins_with_untestet_neighbors);
}

/*! \fn void metroplis_sweep_and_wolff_cluster(gs_graph_t *g, gsl_rng *rng)
    \brief Führt erst einen Metropolis Sweep aus und flipt dann einen
            Wolff Cluster.

    Dies sollte nützlich im Zusammenhang mit parallel Tempering sein.

    \param [in,out]    g    Graph, der modifiziert werden soll
    \param [in]      rng    Zufallszahlengenerator
*/
void metroplis_sweep_and_wolff_cluster(gs_graph_t *g, gsl_rng *rng)
{
    metropolis_monte_carlo_sweep(g, rng);
    wolff_monte_carlo_cluster(g, rng);
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
             const options_t o, double *par_temp_versuche, double *par_temp_erfolge)
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
            if(gsl_rng_uniform(o.rng) < exp(delta))
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
                                int *map_of_temps, const options_t o, const int N)
{
    int j;
    int nT;

    /* Schreibe in Datei */
    if(N > o.t_eq && !(N % o.tau))
    {
        fprintf(data_out_file, "%d ", N);
        for(nT=0;nT<o.num_temps;nT++)
        {
            /* An welcher Stelle liegt die nT-te Temperatur? */
            j = map_of_temps[nT];

            fprintf(data_out_file, "%f %f %f %f ",
                    list_of_graphs[j]->E/list_of_graphs[j]->num_nodes,
                    list_of_graphs[j]->M/list_of_graphs[j]->num_nodes,
                    calculate_wave_vector_dependend_susceptibility_1D(list_of_graphs[j],0),
                    calculate_wave_vector_dependend_susceptibility_1D(list_of_graphs[j],2*M_PI/list_of_graphs[j]->L));
        }
        fprintf(data_out_file, "\n");
    }
}
