/*! \file */
#ifndef _STACK_H
#define _STACK_H

#include <stdlib.h>
#include <stdio.h>

/*! \struct stack_t
    \brief Ein Stack

    Enthält ein Array zum Speichern der Einträge und ein Integer, der
    sich den Index des obersten Elements des Stapels merkt
*/
typedef struct stack_struct
{
    int    *value;                 //!< Array zum Speichern der Einträge
    int     index;                      //!< Index des obersten Elements
    int    length;                         //!< Maximale Höhe des Stacks
} stack_t;

stack_t create_stack(int length);
void push(stack_t *stack, int item);
int pop(stack_t *stack);
int is_empty(stack_t *stack);
void clear_stack(stack_t *stack);

#endif /* _STACK_H */
