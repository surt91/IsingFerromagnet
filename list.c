#include "list.h"

/*      remove_node()
    löscht einen Eintrag aus der Liste
    Parameter:
            head : erstes Element der Liste (Pointer auf Liste)
            elem : pointer auf das zu löschende Element
    Return
        (new) pointer auf den Listenanfang
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

/*      clear_list()
    Löscht eine ganze Liste (zB. Um am Ende den Speicher frei zu geben)
        Parameter:
            head : erstes Element der Liste (Pointer auf Liste)
    Return
            (nothing)
*/
void clear_list(elem_t *head)
{
    while(head != NULL)
        head = remove_node(head, head);
}

/*      create_element()
    Erstellt ein neues Element der Liste. Aus Kapitel 'Listen'
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

/*      delete_element()
    Löscht ein Element der Liste. Aus Kapitel 'Listen'
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

/*      insert_element()
    Fügt ein Element in eine Liste "head" ein, hinter dem Element ort.
    Aus Kapitel 'Listen'
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

/*      print_list()
    Gibt die Liste aus. Aus Kapitel 'Listen'
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
