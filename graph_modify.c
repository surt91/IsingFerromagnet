#include "graph_modify.h"

/*! \fn gs_graph_t **init_graphs(options_t o)
    \brief Diese Funktion initialisiert die Graphen mit allen nötigen
            Informationen

    \param [in] o               struct mit allen wichtigen Informationen
    \return Array mit den initialisierten Graphen
*/
gs_graph_t **init_graphs(const options_t o)
{
    gs_graph_t *g;
    gs_graph_t **list_of_graphs;

    int nT;

    list_of_graphs = (gs_graph_t**) malloc(o.num_temps * sizeof(gs_graph_t*));

    /* Initialisiere den Graphen für die Spins */
    g = gs_create_graph(o.L);

    /* Verschiebe die Knoten */
    move_graph_nodes(g, o.moving_fkt, o.rng, o.sigma);

    /* Verknüpfe die Knoten */
    create_edges(g, o);

    if(o.percolation)
        delete_random_edges_till_percolation(g, o.rng);

    for(nT=0;nT<o.num_temps;nT++)
    {
        /* initialisiere den Status der Spins */
        if(o.start_order == 1)
            init_spins_up(g);
        else
            init_spins_randomly(g, o.rng);

        /* Berechne Energie des Ising Modells */
        g->E = calculate_energy(g);
        /* Berechne Magnetisierung des Ising Modells */
        g->M = calculate_magnetisation(g);

        /* Speichere eine Kopie des Graphen im Array der Graphen */
        list_of_graphs[nT] = gs_shallow_copy_graph(g);

        /* Setze Temperatur */
        list_of_graphs[nT]->T = o.list_of_temps[nT];
    }
    gs_clear_shallow_graph(g);

    return(list_of_graphs);
}

/*! \fn void move_graph_nodes(gs_graph_t *g, double (*f)(const gsl_rng *, double), gsl_rng *rng, const double sigma)
    \brief Verschiebt die Knoten des Graphen g, wobei die Verschiebung zufällig
            nach der Verteilung f mit dem Parameter sigma gewählt wird.

    \param [in,out]   g    Graph, dessen Knoten verschoben werden sollen
    \param [in]       f    Funktion, die Zufallszahlen entsprechend der
                           gewünschten Verteilung liefert
    \param [in]   rng      Zufallszahlengenerator
    \param [in]   sigma    Paramter der Verteilung (zb. Standardabweichung)
*/
void move_graph_nodes(gs_graph_t *g, double (*f)(const gsl_rng *, double), gsl_rng *rng, const double sigma)
{
    int n;
    const int verschiebung_x[9] = {0,0,1,1,1,0,-1,-1,-1};
    const int verschiebung_y[9] = {0,1,1,0,-1,-1,-1,0,1};
    for(n=0;n<g->num_nodes; n++)
    {
        /* Verschiebe Punkte zufällig */
        g->node[n].x += f(rng, sigma);
        g->node[n].y += f(rng, sigma);
        /* Falls die Punkte aus dem Bereich geschoben werden, schiebe sie periodisch wieder rein */
        g->node[n].x += g->L * verschiebung_x[point_not_in_domain(g->node[n].x, g->node[n].y, g->L)];
        g->node[n].y += g->L * verschiebung_y[point_not_in_domain(g->node[n].x, g->node[n].y, g->L)];
    }
}

/*! \fn void create_edges(gs_graph_t *g, void (*graph_cell_border_fkt)(gs_node_t, gs_node_t, double, int, int*, int*, int*, int*), int (*graph_fkt)(double, gs_node_t, gs_node_t, gs_node_t), double (*weighting_fkt)(double, double))
    \brief Fügt Kanten zu einem Graphen hinzu, wodurch ein
            Relative Neighborhood Graph erzeugt werden soll.

    Kantengewichte (J) werden durch eine übergebene Funktion aus
    dem (euklidischen) Abstand der Knoten ermittelt

    \param [in,out]    g    Graph, der modifiziert werden soll
    \param [in]        o    Optionen (weighting Funktion)
*/
void create_edges(gs_graph_t *g, const options_t o)
{
    /*! Prinzip:
        -#  Zuerst kachele einen Graphen, aus neun Kopien des Originalgraphen,
            um die periodischen Randbedingen zu beachten.
            - Index 0 ist der original Graph, der Rest wird im
              Uhrzeigersinn beginnend über dem Original durchnummeriert
        -#  Zeichne die Kanten ein
            - Dies kann durch verschiedene Bedingungsfunktionen zB. für
              einen Relative Neighborhood Graphen oder Gabriel Graph
              angepasst werden.
            - Beim naiven Ansatz (für jedes Knotenpaar alle Knoten
              testen, ob sie im Lune liegen -> O(n^3) ) steigt der
              Rechenaufwand extrem.
            - Hier wird der Cell Teil des ALG-2-CELL Algorithmus
              \cite melchert2013percolation mit dem naiven Ansatz
              kombiniert ("ALG-1-CELL"). Das Gebiet wird in Zellen
              einteilt, die eine Liste aller Knoten in ihrem Bereich
              enthalten. So muss nur eine kleinere Untermenge des
              Graphen getestet werden.
              Dadurch liegt die Laufzeit zwar noch in O(n^3), ist aber
              dennoch deutlich schneller (2-3 Größenordungen auf L=32),
              als der naive Ansatz.
        -#  Berechne aus den Abständen, die dabei
            berechnet werden können die Kantengewichte.
        -#  Lösche die Kopien.
     */
    int i, j, k, l, q, r, s;
    int n, m;
    int x, y;
    int L;
    /* Die folgenden Array, geben an, in welche Richtung der Graph mit
     * gleichem Index verschoben werden soll */
    const int verschiebung_x[9] = {0,0,1,1,1,0,-1,-1,-1};
    const int verschiebung_y[9] = {0,1,1,0,-1,-1,-1,0,1};

    gs_graph_t *gekachelte[9];
    gs_node_t node1, node2, node3;
    double dist12;
    double weight;
    elem_t ***cell_list;
    elem_t *list;

    int found;
    int x0,x1,y0,y1;

    double n_x, n_y;

    L = g->L;

    /* Graphen kacheln */
    gekachelte[0] = g;
    for(i=1;i<9;i++)
        gekachelte[i] = gs_deep_copy_graph(g);

    for(i=1;i<9;i++)
        for(j=0;j<gekachelte[i]->num_nodes;j++)
        {
            gekachelte[i]->node[j].x += verschiebung_x[i]*L;
            gekachelte[i]->node[j].y += verschiebung_y[i]*L;
        }

    /* Allokation und Initialisierung der cell-list vgl. O. Melchert */
    cell_list = (elem_t***) malloc(3*L * sizeof(elem_t**));
    for(i=0;i<3*L;i++)
        cell_list[i] = (elem_t**) malloc(3*L * sizeof(elem_t*));
    for(i=0;i<3*L;i++)
        for(j=0;j<3*L;j++)
            cell_list[i][j] = NULL;

    for(j=0;j<9;j++)
        for(i=0;i<gekachelte[j]->num_nodes;i++)
        {
            n_x = gekachelte[j]->node[i].x;
            n_y = gekachelte[j]->node[i].y;
            n = (int)floor(n_y);
            m = (int)floor(n_x);
            /* Da n und m negativ sein können, werden die Indizes hier
             * verschoben */
            cell_list[n+L][m+L] = insert_element(cell_list[n+L][m+L], create_element(i,j), NULL);
        }

    /* Knoten verbinden */
    /* Versuche jeden Knoten aus g_0 mit allen Knoten aus g_0..8 zu verbinden */
    for(i=0;i<g->num_nodes;i++)                /* Alle Knoten aus g_0 */
    {
        node1 = g->node[i];
        for(j=0;j<9;j++)                       /* Alle Graphen g_0..8 */
            for(k=i+1;k<g->num_nodes;k++)     /* Alle Knoten aus g_0..8,
                                      die noch nicht überprüft wurden */
            {
                found = 0;
                if(k==i)
                    continue;
                node2 = gekachelte[j]->node[k];
                dist12 = sqrt( (node1.x-node2.x)*(node1.x-node2.x)
                                + (node1.y-node2.y)*(node1.y-node2.y) );
                /* Check Kriterium */
                /* Dies muss nur für Knoten in Cells zwischen node1 und
                 * node2 geschehen. Sobald ein Knoten im Lune gefunden
                 * wurde, kann die Suche abgebrochen werden. */
                /* Hier werden die Zellen, die eine rechteckige
                 * "Bounding Box" um den Lune bilden überprüft. */
                /* Berechne Indizes der zu durchsuchenden Cells */

                o.graph_cell_border_fkt(node1, node2, dist12, L, &x0, &x1, &y0, &y1);

                /* Trickreiche Schleife, damit man aus der Mitte startet */
                for(x=(x0+x1)/2,l=0,q=-1;x<=x1 && !found;x-=q*l++,q*=-1)
                    for(y=(y0+y1)/2,s=0,r=-1;y<=y1 && !found;y-=r*s++,r*=-1)
                    {
                        /* Überprüfe die Liste möglicher Knoten */
                        list = cell_list[y+L][x+L];
                        while(list != NULL && !found)
                        {
                            if(list->value == k || list->value == i)
                            {
                                list = list->next;
                                continue;
                            }
                            node3 = gekachelte[list->kachel]->node[list->value];
                            /* teste, ob Knoten im Lune liegt */
                            if(!o.graph_fkt(dist12, node1, node2, node3))
                                found = 1;
                            list = list->next;
                        }
                    }
                if(!found)
                {
                    weight = o.weighting_fkt(o.alpha, dist12);
                    gs_insert_edge(g, i, k, weight);
                }
            }
    }

    /* Kachelung freigeben */
    for(i=1;i<9;i++)
        gs_clear_graph(gekachelte[i]);

    for(i=0;i<3*L;i++)
    {
        for(j=0;j<3*L;j++)
        {
            clear_list(cell_list[i][j]);
        }
        free(cell_list[i]);
    }
    free(cell_list);
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

/*! \fn inline int check_relative_neighborhood(double dist12, gs_node_t node1, gs_node_t node2, gs_node_t node3)
    \brief Prüft, ob node3 im Lune zwischen node1 und node2 liegt

    \param [in]    dist12   Abstand zwischen node1 und node2
    \param [in]    node1    Erster  Knoten
    \param [in]    node2    Zweiter Knoten
    \param [in]    node3    Dritter Knoten
*/
inline int check_relative_neighborhood(double dist12, gs_node_t node1, gs_node_t node2, gs_node_t node3)
{
    double dist13, dist23;
    dist12*=dist12; /* Mache Abstandsquadrat aus dem Abstand */
    /* Überprüfe diesen Punkte */
    dist13 = (node1.x-node3.x)*(node1.x-node3.x)
            + (node1.y-node3.y)*(node1.y-node3.y);
    dist23 = (node2.x-node3.x)*(node2.x-node3.x)
            + (node2.y-node3.y)*(node2.y-node3.y);
    if( (dist13 < dist12 && dist23 < dist12) )
        return(0);
    else
        return(1);
}
/*! \fn inline void get_cell_border_relative_neighborhood(gs_node_t node1, gs_node_t node2, double dist12, int *x0, int *x1, int *y0, int *y1)
    \brief Gibt die "Bounding Box" der zu überprüfenden Knoten an

    \param [in]    node1    Erster  Knoten
    \param [in]    node2    Zweiter Knoten
    \param [in]    dist12   euklidischer Abstand von node1 und node2
    \param [out]   x0       untere x-Grenze
    \param [out]   x1       obere  x-Grenze
    \param [out]   y0       untere y-Grenze
    \param [out]   y1       obere y-Grenze
*/
inline void get_cell_border_relative_neighborhood(gs_node_t node1, gs_node_t node2, double dist12, int L, int *x0, int *x1, int *y0, int *y1)
{
    *x0 = MAX(floor(node1.x-dist12), floor(node2.x-dist12));
    *x1 = MIN( ceil(node1.x+dist12),  ceil(node2.x+dist12));
    *y0 = MAX(floor(node1.y-dist12), floor(node2.y-dist12));
    *y1 = MIN( ceil(node1.y+dist12),  ceil(node2.y+dist12));

    *x0 = MAX(*x0, -L);
    *x1 = MIN(*x1, 2*L-1);
    *y0 = MAX(*y0, -L);
    *y1 = MIN(*y1, 2*L-1);
}

/*! \fn inline int check_gabriel(double dist12, gs_node_t node1, gs_node_t node2, gs_node_t node3)
    \brief Prüft, ob node3 im Kreis zwischen node1 und node2 liegt

    \param [in]    dist12   Abstand zwischen node1 und node2
    \param [in]    node1    Erster  Knoten
    \param [in]    node2    Zweiter Knoten
    \param [in]    node3    Dritter Knoten
*/
inline int check_gabriel(double dist12, gs_node_t node1, gs_node_t node2, gs_node_t node3)
{
    double dist3m;
    double mid_x, mid_y;
    /* Mittelpunkt */
    mid_x = (node1.x+node2.x)/2;
    mid_y = (node1.y+node2.y)/2;
    /* Überprüfe diesen Punkte */
    dist3m = (mid_x-node3.x)*(mid_x-node3.x)
           + (mid_y-node3.y)*(mid_y-node3.y);

    if( (sqrt(dist3m) <= dist12/2) )
        return(0);
    else
        return(1);
}

/*! \fn inline void get_cell_border_gabriel(gs_node_t node1, gs_node_t node2, double dist12, int *x0, int *x1, int *y0, int *y1)
    \brief Gibt die "Bounding Box" der zu überprüfenden Knoten an

    \param [in]    node1    Erster  Knoten
    \param [in]    node2    Zweiter Knoten
    \param [in]    dist12   euklidischer Abstand von node1 und node2
    \param [out]   x0       untere x-Grenze
    \param [out]   x1       obere  x-Grenze
    \param [out]   y0       untere y-Grenze
    \param [out]   y1       obere y-Grenze
*/
inline void get_cell_border_gabriel(gs_node_t node1, gs_node_t node2, double dist12, int L, int *x0, int *x1, int *y0, int *y1)
{
    double mid_x, mid_y;
    mid_x = (node1.x+node2.x)/2;
    mid_y = (node1.y+node2.y)/2;
    *x0 = floor(mid_x-dist12/2);
    *x1 =  ceil(mid_x+dist12/2);
    *y0 = floor(mid_y-dist12/2);
    *y1 =  ceil(mid_y+dist12/2);

    *x0 = MAX(*x0, -L);
    *x1 = MIN(*x1, 2*L-1);
    *y0 = MAX(*y0, -L);
    *y1 = MIN(*y1, 2*L-1);
}


/*! \fn void delete_random_edges_till_percolation(gs_graph_t *g, gsl_rng *rng)
    \brief Diese Funktion löscht eine zufällige Kante aus dem Graphen g

    \param [in] g Graph aus dem eine Kante gelöscht werden soll
*/
void delete_random_edges_till_percolation(gs_graph_t *g, gsl_rng *rng)
{
    int conf = 1; /* 0: einzelner cluster darf aus 1 knoten bestehen
                     1: einzelner cluster muss mindestens 2 Knoten haben*/
    int node1, m, node2;
    double weight;
    /* suche einen zufälligen Knoten, bis du einen mit mehr als einer
     * Kante gefunden hast */
    while(1)
    {
        node1 = gsl_rng_uniform(rng) * g->num_nodes;
        if(g->node[node1].num_neighbors > conf)
        {
            m = gsl_rng_uniform(rng) * g->node[node1].num_neighbors;
            node2 = g->node[node1].neighbors[m].index;
            if(g->node[node2].num_neighbors > conf)
            {
                weight = g->node[node1].neighbors[m].weight;
                gs_remove_edge(g, node1, node2);
                if(!test_connectivity_between_nodes(g, node1, node2))
                {
                    /* mache letzten Schritt rückgänging */
                    gs_insert_edge(g, node1, node2, weight);
                    g->spins[node1] = -1;
                    return;
                }
            }
        }
    }
}

/*! \fn int test_connectivity_between_nodes(gs_graph_t *g, int node1, int node2)
    \brief Diese Funktion prüft, on zwischen node1 und node2 eine Verbindung
            durch Kanten besteht.

    Diese Funktion soll per "Depth First Search" http://en.wikipedia.org/wiki/Depth-first_search
    den Graphen rekursiv durchsuchen.

    \param [in] g     Graph
    \param [in] node1 Knoten 2
    \param [in] node2 Knoten 2
*/
int test_connectivity_between_nodes(gs_graph_t *g, int node1, int node2)
{
    int status;
    node_t *tree_of_tested_nodes;
    tree_of_tested_nodes = NULL;
    tree_of_tested_nodes = gs_depth_first_search(g, node1, node2, tree_of_tested_nodes, &status);
    clear_tree(tree_of_tested_nodes);
    return status;
}

/*! \fn node_t * gs_depth_first_search(gs_graph_t *g, int node, int target, node_t *tree_of_tested_nodes, int *status)
    \brief Diese Funktion soll per "Depth First Search" http://en.wikipedia.org/wiki/Depth-first_search
    den Graphen rekursiv durchsuchen.

    \param [in] g      Graph
    \param [in] node   Momentaner Knoten
    \param [in] target Zielknoten
*/
node_t * gs_depth_first_search(gs_graph_t *g, int node, int target, node_t *tree_of_tested_nodes, int *status)
{
    int i, neighbor, p;
    int tmp = 0;
    node_t *tr_node;

    if(node == target)
    {
        *status = 1;
        return tree_of_tested_nodes;
    }

    /* Mache dfs fur alle Nachbarn von node */
    for(i=0;i<g->node[node].num_neighbors;i++)
    {
        neighbor = g->node[node].neighbors[i].index;
        tr_node = tr_create_node(neighbor);
        p = 0;
        tree_of_tested_nodes = tr_insert_node(tree_of_tested_nodes,tr_node,&p);
        if(p)                     /* node.key schon im Baum vorhanden */
        {
            tr_delete_node(tr_node);
            continue;   /* Muss also nicht mehr berucksichtigt werden */
        }
        gs_depth_first_search(g, neighbor, target, tree_of_tested_nodes, &tmp);
        if(tmp)
        {
            *status = 1;
            return tree_of_tested_nodes;
        }
    }
    /* Nicht gefunden: Graph ist nicht zusammenhangend */
    *status = 0;
    return tree_of_tested_nodes;
}

/*! \fn double get_mean_weight(gs_graph_t *g)
    \brief Diese Funktion gibt den Mittelwert der Kantengewichte im
            Graphen g aus

    \param [in] g Graph über den gemittelt wird
    \return Mittelwert der Kantengewichte
*/
double get_mean_weight(gs_graph_t *g)
{
    int n, i;
    double sum = 0;
    int count = 0;
    for(n=0;n<g->num_nodes; n++)
        for(i=0;i<g->node[n].num_neighbors;i++)
        {
            sum += g->node[n].neighbors[i].weight;
            count++;
        }
    /* Es sollte egal sein, dass über jede Kante zweimal summiert wird */
    return (sum / count);
}

/*! \fn double get_sum_weight(gs_graph_t *g)
    \brief Diese Funktion gibt die Summe der Kantengewichte pro Knoten
            im Graphen g aus

    \param [in] g Graph der untersucht wird
    \return Summe der Kantengewichte pro Knoten
*/
double get_sum_weight(gs_graph_t *g)
{
    int n, i;
    double sum = 0;
    for(n=0;n<g->num_nodes; n++)
        for(i=0;i<g->node[n].num_neighbors;i++)
            sum += g->node[n].neighbors[i].weight;

    return (sum / g->num_nodes);
}

/*! \fn double get_mean_deg(gs_graph_t *g)
    \brief Diese Funktion gibt den mittlere Kantenzahl pro Knoten aus

    \param [in] g Graph über den gemittelt wird
    \return mittlere Kantenzahl pro Knoten aus
*/
double get_mean_deg(gs_graph_t *g)
{
    int n, i;
    double count = 0;
    for(n=0;n<g->num_nodes; n++)
        for(i=0;i<g->node[n].num_neighbors;i++)
            count++;

    return (count/g->num_nodes);
}

/*! \fn void init_spins_randomly(gs_graph_t *g, gsl_rng *rng)
    \brief Weist den Spins im gs_graph_t g zufällige Werte zu

    \param [in,out]    g    Graph, der modifiziert werden soll
    \param [in]      rng    Zufallszahlengenerator
*/
void init_spins_randomly(gs_graph_t *g, gsl_rng *rng)
{
    int num_nodes, i;

    num_nodes = g->num_nodes;

    for(i=0;i<num_nodes;i++)
    {
        if(gsl_rng_uniform(rng)<0.5)
            g->spins[i] = 1;
        else
            g->spins[i] = -1;
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
        g->spins[i] = 1;
}
