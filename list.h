/*! \file */
#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <stdlib.h>

/*! \struct elem_struct
    \brief Ein Listenelement

    Soll genutzt werden, um in einem gewichteten Graphen Nachbarknoten
    anzuzeigen.
    Enthält die beiden Datenfelder, für den Index des Nachbarknotens
    und des Gewichts der Kante.
    Außerdem einen Pointer auf das nächste Listenelement
*/
struct elem_struct
{
    int                index;    //!< Der Index des referenzierten Spins
    double            weight;                     //!< Gewicht der Kante
    struct elem_struct *next;                //!< Pointer zum Nachfolger
};

/*! \var elem_t;
    \brief Eine Abkürzung für struct elem_struct
*/
typedef struct elem_struct elem_t;

elem_t *remove_node(elem_t *head, elem_t *elem);
void clear_list(elem_t *head);
elem_t *create_element(int index, double weight);
elem_t *copy_list(elem_t *list);
int delete_element(elem_t *elem);
elem_t *insert_element(elem_t *head, elem_t *elem, elem_t *ort);
void print_list(elem_t *list);

#endif /* _LIST_H */
