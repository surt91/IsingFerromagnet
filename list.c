#include "list.h"

/*! \fn elem_t *remove_node(elem_t *head, elem_t *elem)
    \brief löscht einen Eintrag aus der Liste

    \param [in,out]  head   erstes Element der Liste (Pointer auf Liste)
    \param [in,out]  elem   pointer auf das zu löschende Element
    \return (new) Pointer auf den Listenanfang
*/
elem_t *remove_node(elem_t *head, elem_t *elem)
{
    elem_t *tmp;
    if(elem==NULL)
    {
        fprintf(stderr, "kann nicht 'NULL' Löschen\n");
    }
    else if(elem==head)
    {
        head = elem->next;
        elem->next = NULL;
        if(delete_element(elem))
            fprintf(stderr, "Freigeben des Elements fehlgeschlagen!\n");
    }
    else
    {
        tmp = head;
        while(tmp->next != elem)
        {
            tmp = tmp->next;
        }
        tmp->next = elem->next;
        elem->next = NULL;
        if(delete_element(elem))
            fprintf(stderr, "Freigeben des Elements fehlgeschlagen!\n");
    }
    return(head);
}

/*! \fn void clear_list(elem_t *head)
    \brief Löscht eine ganze Liste (zB. Um am Ende den Speicher frei
            zu geben)

    \param [in,out]  head   erstes Element der Liste (Pointer auf Liste)
*/
void clear_list(elem_t *head)
{
    while(head != NULL)
        head = remove_node(head, head);
}

/*! \fn elem_t *create_element(int index, double weight)
    \brief  Erstellt ein neues Element der Liste. Aus Kapitel 'Listen'

    \param [in]  index,weight   Inhalt des zu erstellenden Listenelemtes
    \return Listenelement mit passenden Einträgen
*/
elem_t *create_element(int index, double weight)
{
    elem_t *elem;
    elem = (elem_t *) malloc (sizeof(elem_t));
    elem->index = index;
    elem->weight = weight;
    elem->next = NULL;

    return(elem);
}

/*! \fn int delete_element(elem_t *elem)
    \brief  Löscht ein Element der Liste. Aus Kapitel 'Listen'

    Das zu löschende Elemt darf nicht in einer Liste sein, sondern
    sollte vorher durch remove_node() entfernt worden sein

    \param [in,out] elem    Pointer auf das zu löschende Element
    \return Status, ob Löschung erfolgreich war
*/
int delete_element(elem_t *elem)
{
    if(elem == NULL)
    {
        fprintf(stderr, "kann nicht 'NULL' Löschen\n");
        return(1);
    }
    if(elem->next != NULL)
    {
        fprintf(stderr, "kann keinen Konten mit einem Nachfolger löschen\n");
        return(1);
    }
    free(elem);
    return(0);
}

/*! \fn elem_t *insert_element(elem_t *head, elem_t *elem, elem_t *ort)
    \brief  Fügt ein Element in eine Liste "head" ein, hinter dem Element ort.
            Aus Kapitel 'Listen'

    \param [in,out] head    Pointer auf den Listenanfang
    \param [in]     elem    einzufügendes Element
    \param [in,out] ort     Element, hinter das eingefügt werden soll
    \return (new) Pointer auf den Listenanfang
*/
elem_t *insert_element(elem_t *head, elem_t *elem, elem_t *ort)
{
    if(ort==NULL)                           /*Füge am Anfang (head) ein*/
    {
        elem->next = head;
        head = elem;
    }
    else
    {
        elem->next = ort->next;
        ort->next = elem;
    }
    return(head);
}

/*! \fn void print_list(elem_t *list)
    \brief  Gibt die Liste aus. Aus Kapitel 'Listen'

    \param [in,out] list    Pointer auf den Listenanfang
*/
void print_list(elem_t *list)
{
    while(list != NULL)
    {
        printf("index:  %3d ", list->index);
        printf("weight: %5.2f \n", list->weight);
        list = list->next;
    }
    printf("\n");
}
