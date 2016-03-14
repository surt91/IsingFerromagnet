#include "union_find.h"

// forest[x] < 0 -> root mit abs(forest[x]) Knoten
// forest[x] >= 0 -> parent ist an forest[forest[x]]

/*! \fn union_find_t *uf_create(int N)
    \brief Erstellt eine neue Union Find Struktur

    \param [in]     N   Anzahl der Elemente
    \return erzeugte Union Find Struktur
*/
union_find_t *uf_create(int N)
{
    int i;
    union_find_t *uf;

    uf = (union_find_t*) malloc(sizeof(union_find_t));
    uf->N = N;
    uf->forest = (int*) malloc(N*sizeof(int));

    for(i=0; i<N; ++i)
        uf->forest[i] = -1;

    return uf;
}

/*! \fn void uf_delete(union_find_t *uf)
    \brief Löscht die Union Find Struktur

    \param [in,out] uf   Struktur, die gelöscht werden soll
*/
void uf_delete(union_find_t *uf)
{
    free(uf->forest);
    free(uf);
}

/*! \fn void uf_union(union_find_t *uf, int x, int y)
    \brief Vereinigt zwei Baume in der Union Find Struktur

    \param [in,out] uf   Struktur, die geandert werden soll
    \param [in]     x    Element 1, das vereinigt werden soll
    \param [in]     y    Element 2, das vereinigt werden soll
*/
void uf_union(union_find_t *uf, int x, int y)
{
    int root1 = uf_find(uf, x);
    int root2 = uf_find(uf, y);

    // are already in one Cluster
    if(root1 == root2)
        return;

    // weighted -> append shorter Tree to longer
    // negative Zahlen -> Vergleichszeichen unintuitiv
    int p, c;
    if(uf->forest[root1] < uf->forest[root2])
    {
        p = root1;
        c = root2;
    }
    else
    {
        p = root2;
        c = root1;
    }

    uf->forest[p] += uf->forest[c];
    uf->forest[c] = p;
}

/*! \fn int uf_find(union_find_t *uf, int x)
    \brief Findet Reprasentanten von Element x

    \param [in,out] uf   Struktur, die durchsucht werden soll
    \param [in]     x    Element, dessen Reprasentatn gefunden werden soll
*/
int uf_find(union_find_t *uf, int x)
{
    // root found
    if(uf->forest[x] < 0)
        return x;

    // search recursive
    int root = uf_find(uf, uf->forest[x]);

    // path compression
    uf->forest[x] = root;

    return root;
}
