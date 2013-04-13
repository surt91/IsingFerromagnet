#include "stack.h"

/*! \fn stack_t *push(stack_t *stack, int item);
    \brief Legt ein neues Element auf den Stack

    \param [in]  stack  erstes Element des Stacks (Pointer auf Stack)
    \param [in]      item   Wert, der auf den Stack geleget werden soll
    \return (new) Pointer auf den Stack
*/
stack_t *push(stack_t *stack, int item)
{
    stack_t *elem;
    elem = (stack_t *) malloc (sizeof(stack_t));
    elem->value = item;
    elem->next = stack;

    return(elem);
}

/*! \fn stack_t *pop(stack_t *stack, int *item)
    \brief Nimmt das oberste Element vom Stack und liefert seinen Wert

    \param [in]  stack erstes Element des Stacks (Pointer auf Stack)
    \param [out] item  Zeiger auf die Variable, die den Wert speichern soll
    \return (new) Pointer auf den Stack
*/
stack_t *pop(stack_t *stack, int *item)
{
    stack_t *tmp;

    if(item != NULL)
        *item = stack->value;

    tmp = stack;
    stack = stack->next;

    free(tmp);

    return(stack);
}

/*! \fn int is_empty(stack_t *stack);
    \brief Prüft, ob der Stack leer ist

    \param [in]  stack erstes Element des Stacks (Pointer auf Stack)
    \return (new) Bool, ob der Stack leer ist
*/
int is_empty(stack_t *stack)
{
    if(stack == NULL)
        return(1);
    else
        return(0);
}

/*! \fn void clear_stack(stack_t *stack)
    \brief Löscht einen ganzen Stack (zB. Um am Ende den Speicher frei
            zu geben)

    \param [in,out]  stack   erstes Element der Liste (Pointer auf Liste)
*/
void clear_stack(stack_t *stack)
{
    while(!is_empty(stack))
        stack = pop(stack, NULL);
}
