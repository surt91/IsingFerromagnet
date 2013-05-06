#include "list.h"

/*! \fn remove_node()
    \brief löscht einen Eintrag aus der Liste

    \param [in] head erstes Element der Liste (Pointer auf Liste)
    \param [in] elem pointer auf das zu löschende Element
    \return (new) pointer auf den Listenanfang
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

/*! \fn clear_list()
    \brief Löscht eine ganze Liste (zB. Um am Ende den Speicher frei zu geben)

    \param [in] head erstes Element der Liste (Pointer auf Liste)
*/
void clear_list(elem_t *head)
{
    while(head != NULL)
        head = remove_node(head, head);
}

/*! \fn create_element()
    \brief Erstellt ein neues Element der Liste. Aus Kapitel 'Listen'

    \param [in] value  Inhalt
    \param [in] kachel Inhalt
    \return (new) pointer auf den Listenanfang
*/
elem_t *create_element(int value, int kachel)
{
    elem_t *elem;
    elem = (elem_t *) malloc (sizeof(elem_t));
    elem->value = value;
    elem->kachel = kachel;
    elem->next = NULL;

    return(elem);
}

/*! \fn delete_element()
    \brief Löscht ein Element der Liste. Aus Kapitel 'Listen'

    \param [in] elem zu löschendes Element
    \return Status
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

/*! \fn insert_element()
    \brief Fügt ein Element in eine Liste "head" ein, hinter dem Element ort.
    Aus Kapitel 'Listen'

    \param [in] head Listenanfang
    \param [in] elem Element, das hinzugefügt werden soll
    \param [in] ort  stelle, an der es hinzugefügt werden soll
    \return (new) pointer auf den Listenanfang
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

/*! \fn print_list()
    \brief Gibt die Liste aus. Aus Kapitel 'Listen'

    \param [in] list Liste, die Gedruckt werden soll
*/
void print_list(elem_t *list)
{
    while(list != NULL)
    {
        printf("%d ", list->value);
        list = list->next;
    }
    printf("\n");
}
