#include "graph.h"

/*! \fn gs_graph_t *gs_create_graph(int num_nodes)
    \brief Erstellt einen neuen Graphen. Aus Kapitel 'Graphs'

    \param [in]     L   Kantenlänge des Gitters
    \param [in]     f   Funktion, die eine Abweichung der Positionen liefert
    \param [in] sigma   Parameter, der die Verschiebungen der x und y
                        bestimmt (zB. Breite der Glockenkurve)
    \return erzeugter Graph
*/
gs_graph_t *gs_create_graph(int L, double (*f)(double), double sigma)
{
    gs_graph_t *g;
    int n;
    int num_nodes;

    num_nodes = L*L;

    g = (gs_graph_t *) malloc(sizeof(gs_graph_t));
    g->num_nodes = num_nodes;
    g->L = L;
    g->node = (gs_node_t *) malloc(num_nodes*sizeof(gs_node_t));

    for(n=0;n<num_nodes; n++)
    {
        g->node[n].neighbors = NULL;
        g->node[n].x = n % L + f(sigma);
        g->node[n].y = n / L + f(sigma);
    }

    return(g);
}

/*! \fn void gs_insert_edge(gs_graph_t *g, int from, int to, double weight)
    \brief Erstellt eine neue Kante im Graphen g. Aus Kapitel 'Graphs'

    \param [in,out]    g    Graph, der modifiziert werden soll
    \param [in]     from    Nummer des Knoten #1, der verbunden werden soll
    \param [in]       to    Nummer des Knoten #2, der verbunden werden soll
    \param [in]   weight    Gewicht der Kante
*/
void gs_insert_edge(gs_graph_t *g, int from, int to, double weight)
{
    elem_t *elem1, *elem2;

    /* durchsuche den Graphen, ob die Kante schon existiert */
    if(gs_edge_exists(g, from, to))
        return;                  /*Kante existiert, Also ist der graph fertig */

    /* Erzeuge Listenelemente, die dann in die Liste eingefügt werden */
    elem1 = create_element(to, weight);
    g->node[from].neighbors =
            insert_element(g->node[from].neighbors, elem1, NULL);
    elem2 = create_element(from, weight);
    g->node[to].neighbors =
            insert_element(g->node[to].neighbors, elem2, NULL);
}

/*! \fn int gs_edge_exists(gs_graph_t *g, int from, int to)
    \brief Stellt fest, ob eine Kante zwischen from und to besteht

    \param [in]    g    Graph, der überprüft werden soll
    \param [in] from    Nummer des Knoten #1
    \param [in]   to    Nummer des Knoten #2
    \return 1: ja, 0: nein
*/
int gs_edge_exists(gs_graph_t *g, int from, int to)
{
    elem_t *list;

    /* durchsuche den Graphen, ob die Kante schon existiert */
    list = g->node[from].neighbors;
    while(list != NULL)
    {
        if(list->index == to)
            return(1);
        list = list->next;
    }
    return(0);
}

/*! \fn void gs_clear_graph(gs_graph_t *g)
    \brief Löscht eine Kante im Graphen g.

    \param [in,out] g   Graph, der gelöscht werden soll
*/
void gs_clear_graph(gs_graph_t *g)
{
    int num,i;

    num = g->num_nodes;
    for(i=0;i<num;i++)
    {
        clear_list(g->node[i].neighbors);
    }
    free(g->node);
    free(g);
}

/*! \fn void print_graph_for_graph_viz(gs_graph_t *g, int id)
    \brief gibt den Graphen als Input_datei für GraphViz

    Aufruf von GraphViz ist bspw. möglich mit dem Kommando

        neato -Tpng test.dot > testgraph.png

    \param g   : Array des Graphs
*/
void print_graph_for_graph_viz(gs_graph_t *g)
{
    int i=0, num_nodes = g->num_nodes;
    elem_t *list;

    int L = g->L;

    printf("strict graph test\n");
    printf("{\n");
    for(i=0;i<num_nodes;i++)
    {
        printf("%d [pos=\"%f,%f!\"];\n", i, g->node[i].x, g->node[i].y);
        list = g->node[i].neighbors;
        while(list != NULL)
        {
            /* Drucke keine periodischen Randbedingungen */
            if(i+1 == list->index || i - 1 == list->index || i - L == list->index || i + L == list->index)
                printf("%d -- %d\n", i, list->index);
            list = list->next;
        }
    }
    printf("}\n\n");
}

/*! \fn void print_graph_for_gnuplot(gs_graph_t *g)
    \brief gibt den Graphen als Datendatei für gnuplot auf stdout aus

    Aufruf von Gnuplot ist bspw. möglich mit dem Kommando

        plot "out.dat" w p

    bis jetzt noch ohne Nachbarn, also nur die Positionen der Knoten

    \param g   : Array des Graphs
*/
void print_graph_for_gnuplot(gs_graph_t *g)
{
    int i=0, num_nodes = g->num_nodes;

    for(i=0;i<num_nodes;i++)
    {
        printf("%f %f\n", g->node[i].x, g->node[i].y);
    }
}
