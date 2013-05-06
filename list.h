/*! \file */
#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <stdlib.h>

/*! \struct elem_struct
    \brief Listenelement eine einfachen linked list

    Dabei nimmt value den Index des Knotens auf und Kachel den Index des
    Graphen.
*/
struct elem_struct
{
    int                value;    //!< Der Inhalt
    int               kachel;    //!< Der Inhalt
    struct elem_struct *next;    //!< Pointer zum Nachfolger (NULL, wenn letzter)
};

typedef struct elem_struct elem_t;

elem_t *remove_node(elem_t *head, elem_t *elem);

void clear_list(elem_t *head);
elem_t *create_element(int value, int kachel);
int delete_element(elem_t *elem);
elem_t *insert_element(elem_t *head, elem_t *elem, elem_t *ort);
void print_list(elem_t *list);

#endif /* _LIST_H */
