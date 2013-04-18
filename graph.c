#include "graph.h"

/*! \fn gs_graph_t *gs_create_graph(int L)
    \brief Erstellt einen neuen Graphen. Aus Kapitel 'Graphs'

    \param [in]     L   Kantenlänge des Gitters
    \return erzeugter Graph
*/
gs_graph_t *gs_create_graph(int L)
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
        g->node[n].num_neighbors = 0;
        g->node[n].x = n % L;
        g->node[n].y = n / L;
    }

    return(g);
}

/*! \fn gs_graph_t *gs_copy_graph(gs_graph_t *g)
    \brief Liefert eine Kopie des Graphen g

    \param [in]     g   zu kopierender Graph
    \return kopierter Graph
*/
gs_graph_t *gs_copy_graph(gs_graph_t *g)
{
    gs_graph_t *copy;
    int n, i;

    copy = (gs_graph_t *) malloc(sizeof(gs_graph_t));
    copy->num_nodes = g->num_nodes;
    copy->L = g->L;
    copy->node = (gs_node_t *) malloc(g->num_nodes*sizeof(gs_node_t));

    for(n=0;n<g->num_nodes; n++)
    {
        copy->node[n].num_neighbors = g->node[n].num_neighbors;
        copy->node[n].neighbors = (gs_edge_t *) malloc(g->node[n].num_neighbors * sizeof(gs_edge_t));
        for(i=0;i<g->node[n].num_neighbors;i++)
            copy->node[n].neighbors[i] = g->node[n].neighbors[i];
        copy->node[n].x = g->node[n].x;
        copy->node[n].y = g->node[n].y;
        copy->node[n].spin = g->node[n].spin;
    }
    copy->T = g->T;
    copy->E = g->E;
    copy->M = g->M;

    return(copy);
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
    /* durchsuche den Graphen, ob die Kante schon existiert */
    if(gs_edge_exists(g, from, to))
        return;          /*Kante existiert, Also ist der graph fertig */

    g->node[from].num_neighbors++;
    g->node[from].neighbors = (gs_edge_t *) realloc (g->node[from].neighbors, g->node[from].num_neighbors * sizeof(gs_edge_t));
    g->node[from].neighbors[g->node[from].num_neighbors-1].index = to;
    g->node[from].neighbors[g->node[from].num_neighbors-1].weight = weight;

    g->node[to].num_neighbors++;
    g->node[to].neighbors = (gs_edge_t *) realloc (g->node[to].neighbors, g->node[to].num_neighbors * sizeof(gs_edge_t));
    g->node[to].neighbors[g->node[to].num_neighbors-1].index = from;
    g->node[to].neighbors[g->node[to].num_neighbors-1].weight = weight;
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
    int n;
    gs_edge_t *list;

    /* durchsuche den Graphen, ob die Kante schon existiert */
    list = g->node[from].neighbors;
    for(n=0;n<g->node[from].num_neighbors;n++)
    {
        if(list[n].index == to)
            return(1);
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
        free(g->node[i].neighbors);
    }
    free(g->node);
    free(g);
}

/*! \fn void print_graph_for_graph_viz(gs_graph_t *g)
    \brief gibt den Graphen als Input_datei für GraphViz

    Aufruf von GraphViz ist bspw. möglich mit dem Kommando

        neato -Tpng test.dot > testgraph.png

    \param g   : Array des Graphs
*/
void print_graph_for_graph_viz(gs_graph_t *g)
{
    int i=0, num_nodes = g->num_nodes;
    int n;
    gs_edge_t *list;

    int L = g->L;

    printf("strict graph test\n");
    printf("{\n");
    for(i=0;i<num_nodes;i++)
    {
        if(g->node[i].spin == 1)
            printf("%d [pos=\"%f,%f!\",label=\"\",shape=\"circle\",\
                fillcolor=\"black\",style=\"filled\"];\n", i, g->node[i].x, g->node[i].y);
        else
            printf("%d [pos=\"%f,%f!\",label=\"\",shape=\"circle\",\
                fillcolor=\"white\",style=\"filled\"];\n", i, g->node[i].x, g->node[i].y);
        /* printf("%d [pos=\"%f,%f!\",label=\"%d\",shape=\"circle\"];\n", i, g->node[i].x, g->node[i].y, g->node[i].spin); */
        list = g->node[i].neighbors;
        for(n=0;n<g->node[i].num_neighbors;n++)
        {
            /* Drucke keine periodischen Randbedingungen */
            if(i+1 == list[n].index || i - 1 == list[n].index || i - L == list[n].index || i + L == list[n].index)
                //~ printf("%d -- %d [label=\" %.2f\"]\n", i, list->index, list->weight);
                printf("%d -- %d [penwidth=\" %.2f\"]\n", i, list[n].index, list[n].weight*10);
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
