#include "stack.h"

/*! \fn stack_t create_stack(int length)
    \brief Initialisert einen leeren Stack

    \param [in]  length     Länge des Stacks
    \return (new) Pointer auf den Stack
*/
stack_t create_stack(int length)
{
    stack_t stack;
    stack.value = (int*) malloc(length * sizeof(int));
    stack.index = 0;
    stack.length = length;
    return(stack);
}

/*! \fn stack_t *push(stack_t *stack, int item);
    \brief Legt ein neues Element auf den Stack

    \param [in,out]  stack   Stack
    \param [in]      item   Wert, der auf den Stack geleget werden soll
*/
void push(stack_t *stack, int item)
{
    if( stack->index < stack->length )
        stack->value[stack->index++] = item;
    else
    {
        fprintf(stderr,"Stackoverflow ;)");
        exit(-1);
    }
}

/*! \fn int pop(stack_t *stack)
    \brief Nimmt das oberste Element vom Stack und liefert seinen Wert

    \param [in,out]  stack Stack
    \return Wert des obersten Stackelements
*/
int pop(stack_t *stack)
{
    if(stack->index > 0)
        return(stack->value[--stack->index]);
    else
    {
        fprintf(stderr,"Stackunderflow ;)");
        exit(-1);
    }

}

/*! \fn int is_empty(stack_t *stack);
    \brief Prüft, ob der Stack leer ist

    \param [in]  stack Stack
    \return (new) Bool, ob der Stack leer ist
*/
int is_empty(stack_t *stack)
{
    if(stack->index <= 0)
        return(1);
    else
        return(0);
}

/*! \fn void clear_stack(stack_t *stack)
    \brief Löscht einen ganzen Stack (zB. Um am Ende den Speicher frei
            zu geben)

    \param [in,out]  stack   Stack
*/
void clear_stack(stack_t *stack)
{
    free(stack->value);
}
