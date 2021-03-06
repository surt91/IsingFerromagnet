/*! \file */
#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdlib.h>
#include <stdio.h>

/*! \struct queue_node_t
    \brief Eine linked list

    Enthält eine linked list
*/
struct queue_node_struct
{
    struct queue_node_struct *next;    //!< Pointer auf nächstes Element
    int                      value;                          //!< Inhalt
};

typedef struct queue_node_struct queue_node_t;

/*! \struct queue_t
    \brief Eine Queue

    Enthält eine linked list und Pointer auf Anfang und Ende
*/
typedef struct
{
    struct queue_node_struct *first;
    struct queue_node_struct *last;
    int size;
} queue_t;

queue_t *create_queue();
void enqueue(queue_t *queue, int value);
int dequeue(queue_t *queue);
void clear_queue(queue_t *queue);


#endif /* _QUEUE_H */
