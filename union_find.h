/*! \file */
#ifndef _UNIONFIND_H
#define _UNIONFIND_H

#include <stdio.h>
#include <stdlib.h>

/*! \struct union_find_t;
    \brief Union Find Datenstruktur
*/
typedef struct
{
    int N;                                        //!< Anzahl der Knoten
    int* forest;               //!< Darstellung des Disjoint Set Forests
} union_find_t;

union_find_t *uf_create(int N);
void uf_delete(union_find_t *uf);

void uf_add(union_find_t *uf, int x);
void uf_union(union_find_t *uf, int x, int y);
int uf_find(union_find_t *uf, int x);

#endif
