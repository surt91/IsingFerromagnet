/*! \file */
#ifndef _STACK_H
#define _STACK_H

#include <stdlib.h>

/*! \struct stack_struct
    \brief Ein Stackelement

    Enthält ein integer Speicherfeld und einen Link auf das nächste Stack
    Element.
*/
struct stack_struct
{
    int                value;                        //!< Der Nutzinhalt
    struct stack_struct *next;                //!< Pointer zum Nachfolger
};

/*! \var stack_t;
    \brief Eine Abkürzung für struct elem_struct
*/
typedef struct stack_struct stack_t;

stack_t *push(stack_t *stack, int item);
stack_t *pop(stack_t *stack, int *item);
int is_empty(stack_t *stack);
void clear_stack(stack_t *stack);

#endif /* _LIST_H */
