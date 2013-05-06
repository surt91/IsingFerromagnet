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
    g->spins = (short int *) malloc(g->num_nodes*sizeof(short int));

    for(n=0;n<num_nodes; n++)
    {
        g->node[n].neighbors = NULL;
        g->node[n].num_neighbors = 0;
        g->node[n].x = n % L;
        g->node[n].y = n / L;
    }

    return(g);
}

/*! \fn gs_graph_t *gs_shallow_copy_graph(gs_graph_t *g)
    \brief Liefert eine Kopie des Graphen g, wobei aber die Knoten (also
    die Struktur des Graphen mit dem Original geteilt wird)

    \param [in]     g   zu kopierender Graph
    \return kopierter Graph
*/
gs_graph_t *gs_shallow_copy_graph(gs_graph_t *g)
{
    gs_graph_t *copy;
    int n;

    copy = (gs_graph_t *) malloc(sizeof(gs_graph_t));
    copy->num_nodes = g->num_nodes;
    copy->L = g->L;
    copy->node = g->node;
    copy->spins = (short int *) malloc(g->num_nodes*sizeof(short int));

    for(n=0;n<g->num_nodes; n++)
    {
        copy->spins[n] = g->spins[n];
    }
    copy->T = g->T;
    copy->E = g->E;
    copy->M = g->M;

    return(copy);
}

/*! \fn gs_graph_t *gs_deep_copy_graph(gs_graph_t *g)
    \brief Liefert eine Kopie des Graphen g

    \param [in]     g   zu kopierender Graph
    \return kopierter Graph
*/
gs_graph_t *gs_deep_copy_graph(gs_graph_t *g)
{
    gs_graph_t *copy;
    int n, i;

    copy = (gs_graph_t *) malloc(sizeof(gs_graph_t));
    copy->num_nodes = g->num_nodes;
    copy->L = g->L;
    copy->node = (gs_node_t *) malloc(g->num_nodes*sizeof(gs_node_t));
    copy->spins = (short int *) malloc(g->num_nodes*sizeof(short int));

    for(n=0;n<g->num_nodes; n++)
    {
        copy->node[n].num_neighbors = g->node[n].num_neighbors;
        copy->node[n].neighbors = (gs_edge_t *) malloc(g->node[n].num_neighbors * sizeof(gs_edge_t));
        for(i=0;i<g->node[n].num_neighbors;i++)
            copy->node[n].neighbors[i] = g->node[n].neighbors[i];
        copy->node[n].x = g->node[n].x;
        copy->node[n].y = g->node[n].y;
        copy->spins[n] = g->spins[n];
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
    free(g->spins);
    free(g);
}
/*! \fn void gs_clear_shallow_graph(gs_graph_t *g)
    \brief Löscht eine Kante im Graphen g.

    \param [in,out] g   Graph, der gelöscht werden soll
*/
void gs_clear_shallow_graph(gs_graph_t *g)
{
    free(g->spins);
    free(g);
}

/*! \fn void svg_circle(double x, double y, int filled, FILE *file)
    \brief Schreibt einen Kreis in die gegebene Datei im SVG Format

    \param x       x-Koordinate des Kreis Mittelpunkts
    \param y       y-Koordinate des Kreis Mittelpunkts
    \param filled  Soll der Kreis gefüllt sein? 1: gefüllt -1: nicht gefüllt
    \param file    File Handler der Output Datei
*/
void svg_circle(double x, double y, int filled, FILE *file)
{
    if(filled == 1)
        fprintf(file, "<circle cx='%f' cy='%f' r='8' stroke='black'\
              stroke-width='2' fill='black'/>\n", (x+1)*40, (y+1)*40);
    else if(filled == -1)
        fprintf(file, "<circle cx='%f' cy='%f' r='8' stroke='black'\
              stroke-width='2' fill='white'/>\n", (x+1)*40, (y+1)*40);
    else
        fprintf(file, "<circle cx='%f' cy='%f' r='8' stroke='black'\
              stroke-width='2' stroke-dasharray='2,3' fill='white'/>\n",
                                                    (x+1)*40, (y+1)*40);
}

/*! \fn void svg_circle(double x, double y, int filled, FILE *file)
    \brief Schreibt einen Kreis in die gegebene Datei im SVG Format

    \param x1      x-Anfangspunkt der Linie
    \param x2      x-Endpunkt der Linie
    \param y1      y-Anfangspunkt der Linie
    \param y2      y-Endpunkt der Linie
    \param file    File Handler der Output Datei
*/
void svg_line(double x1, double x2, double y1, double y2, FILE *file)
{
    fprintf(file, "<line x1='%f' x2='%f' y1='%f' y2='%f' \
                        stroke='black' stroke-width='2' />\n",
                        (x1+1)*40, (x2+1)*40, (y1+1)*40, (y2+1)*40);
}

/*! \fn int point_not_in_domain(double x, double  y, int L)
    \brief Liefert das der Acht Felder um den Graphen, in dem sich der
            Punkt befindet.

    \param x      x-Koordinate des Punkts
    \param y      x-Koordinate des Punkts
    \param L      Kantenlänge des Graphen
*/
int point_not_in_domain(double x, double  y, int L)
{
    int verschiebung_x[9] = {0,0,1,1,1,0,-1,-1,-1};
    int verschiebung_y[9] = {0,1,1,0,-1,-1,-1,0,1};
    double tmp_x, tmp_y;
    int j;

    for(j=1;j<9;j++)
    {
        tmp_x = x + L*verschiebung_x[j];
        tmp_y = y + L*verschiebung_y[j];
        /* Wenn der verschobene Punkt drin liegt, liegt der
         * "originale" nicht drin */
        if(tmp_x > 0 && tmp_x < L && tmp_y > 0 && tmp_y < L)
        {
            return(j);
        }
    }
    return(0);
}

/*! \fn void print_graph_svg(gs_graph_t *g, char* svg_filename)
    \brief gibt den Graphen als SVG aus

    \param  g               Array des Graphs
    \param  svg_filename    Name der Output SVG Datei
*/
void print_graph_svg(gs_graph_t *g, char* svg_filename)
{
    int i, n ,j;
    int L;
    int verschiebung_x[9] = {0,0,1,1,1,0,-1,-1,-1};
    int verschiebung_y[9] = {0,1,1,0,-1,-1,-1,0,1};
    double dx, dy, dx2, dy2;
    double x, y;
    double n1_x, n1_y, n2_x, n2_y;
    double off;
    int n2_i;
    double tmp_length;
    int tmp_idx;
    gs_edge_t *list;
    FILE *svg_file;

    L=g->L;

    svg_file = fopen(svg_filename, "w");
    if(svg_file == NULL)
    {
        fprintf(stderr,"ERROR: %s kann nicht geöffnet werden",svg_filename);
        exit(-1);
    }

    /* Schreibe Header */
    fprintf(svg_file, "<?xml version='1.0' encoding='UTF-8'?> \n\
                        <!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN' 'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\n\
                        <svg xmlns='http://www.w3.org/2000/svg'\n\
                        xmlns:xlink='http://www.w3.org/1999/xlink' xmlns:ev='http://www.w3.org/2001/xml-events'\n\
                        version='1.1' baseProfile='full'>\n");

    /* Rahmen */
    off = 0;
    svg_line(   0+off, g->L+off,    0+off,    0+off, svg_file);
    svg_line(g->L+off, g->L+off,    0+off, g->L+off, svg_file);
    svg_line(g->L+off,    0+off, g->L+off, g->L+off, svg_file);
    svg_line(   0+off,    0+off,    0+off, g->L+off, svg_file);

    /* Kanten */
    for(i=0;i<g->num_nodes;i++)
    {
        n1_x = g->node[i].x;
        n1_y = g->node[i].y;
        n1_x += L * verschiebung_x[point_not_in_domain(n1_x, n1_y, L)];
        n1_y += L * verschiebung_y[point_not_in_domain(n1_x, n1_y, L)];

        list = g->node[i].neighbors;
        for(n=0;n<g->node[i].num_neighbors;n++)
        {
            n2_i = list[n].index;
            n2_x = g->node[n2_i].x;
            n2_y = g->node[n2_i].y;
            n2_x += L * verschiebung_x[point_not_in_domain(n2_x, n2_y, L)];
            n2_y += L * verschiebung_y[point_not_in_domain(n2_x, n2_y, L)];
            tmp_length = L;
            tmp_idx  = 0;
            /* Berücksichtige periodischen Randbedingungen */
            for(j=1;j<9;j++)
            {
                dx = n1_x - n2_x;
                dy = n1_y - n2_y;
                dx2 = dx + L*verschiebung_x[j];
                dy2 = dy + L*verschiebung_y[j];
                if(dx*dx+dy*dy > dx2*dx2+dy2*dy2)    /* Such kürzeste */
                    if(tmp_length > dx2*dx2+dy2*dy2)         /* Kante */
                    {
                        tmp_length = dx2*dx2+dy2*dy2;
                        tmp_idx    = j;
                    }
            }
            if(tmp_idx) /* Kante zeigt auf periodischen Punkt */
            {
                x = n2_x - L*verschiebung_x[tmp_idx];
                y = n2_y - L*verschiebung_y[tmp_idx];
                svg_line(n1_x, x, n1_y, y, svg_file);
                /* zeichne zusätzlich den periodischen Punkt */
                svg_circle(x, y , 0, svg_file);
            }
            else
                svg_line(n1_x, n2_x, n1_y, n2_y, svg_file);
        }
    }
    /* Knoten */ /* (Seperat, damit keine Kante über einem Knoten ist) */
    for(i=0;i<g->num_nodes;i++)
    {
        n1_x = g->node[i].x;
        n1_y = g->node[i].y;
        n1_x += L * verschiebung_x[point_not_in_domain(n1_x, n1_y, L)];
        n1_y += L * verschiebung_y[point_not_in_domain(n1_x, n1_y, L)];
        svg_circle(n1_x, n1_y, g->spins[i], svg_file);

    }

    /* Schreibe Footer */
    fprintf(svg_file, "</svg>");
    fclose(svg_file);
}
