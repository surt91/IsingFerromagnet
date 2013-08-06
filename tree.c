#include "tree.h"

/*! \fn node_t *tr_remove_node(node_t *tree, int key_rem, node_t **node_p)
    \brief entfernt einen Knoten ohne die Ordnung zu stören

    \param [in]  tree    erstes Element des Baums
    \param [in]  key_rem Key, der entfernt werden soll
    \param [out] node_p  Adresse des zu entfernenden Knotens (NULL, wenn nicht gefunden)
    \return (new) pointer zur Wurzel
*/
node_t *tr_remove_node(node_t *tree, int key_rem, node_t **node_p)
{
    int n_childs;
    node_t *node_rem, *child1 = NULL, *parent, *cur;

    /* Finde Knoten, der entfernt werden soll */
    node_rem = tr_find_node(tree, key_rem);

    /* Knoten existiert nicht */
    if(node_rem == NULL)
    {
        fprintf(stderr, "Knoten existiert nicht.\n");
        *node_p = NULL;
        return(tree);
    }

    /* Ermittele die Anzahl der Kinder */
    if(node_rem->left == NULL && node_rem->right == NULL)
        n_childs = 0;
    else if(node_rem->left == NULL || node_rem->right == NULL)
        n_childs = 1;
    else
        n_childs = 2;

    /* Finde den Eltern Knoten, damit er die Kinder des gelöschten "adoptiert" */
    parent = tr_find_parent(tree, node_rem);

    switch(n_childs)
    {
        case 2:
            /* Knoten hat zwei Kinder */
            /* Wir müssen den Baum etwas drehen. Dazu suche im rechten Unterbaum
                des Knotens nach dem kleinsten (linkesten) Element, und tausche
                die Keys mit dem Knoten, fahre beim linkesten fort mit case 1,
                da dieser nur noch max ein rechtes child haben kann */
            cur = node_rem->right;
            while(cur->left != NULL)
                cur = cur->left;               /* cur wird hier zum linkesten
                                                         des rechten Teilbaums*/
            /* Jetzt wird dem zu löschenden Knoten der Key vom linkesten
                Knoten zugewiesen und der linkeste (cur) gelöscht. Dazu
                müssen wir vorher noch den Elternknoten von cur finden */
            parent = tr_find_parent(tree, cur);

            node_rem->key = cur->key;

            node_rem = cur;           /* Es soll der linkeste gelöscht werden */

        case 1:
            /* Knoten hat ein Kind */
            /* Gebe dem Kind einen neuen Eltern Knoten und lösche den Link vom
                Knoten zum Kind, fahre fort mit case 0 */
            if(node_rem->left != NULL)
                child1 = node_rem->left;
            else
                child1 = node_rem->right;
            node_rem->left = NULL;
            node_rem->right = NULL;

        case 0:
            /* Knoten hat keine Kinder */
            /* Lösche den Knoten und setzte den Link des Elternteils auf NULL
                bzw. Kind des gelöschten Knotens */
            if(parent == NULL)                       /* Knoten ist die Wurzel*/
                tree = child1;
            else if(parent->left == node_rem)
                parent->left = child1;
            else
                parent->right = child1;
    }
    *node_p = node_rem;
    return(tree);
}

/*! \fn node_t *tr_find_parent(node_t *tree, node_t *node)
    \brief Suche nach dem Elternteil des Knotens node
    \param tree  erstes Element des Baums
    \param node  Knoten, dessen Elternteil gefunden werden soll
    \return (new) pointer auf Elternteil
*/
node_t *tr_find_parent(node_t *tree, node_t *node)
{
    node_t *cur = tree;
    while(1)
    {
        if(cur->key > node->key)
        {
            if(cur->left == node)
                return(cur);                         /* Knoten links gefunden */
            cur = cur->left;
        }
        else if(cur->key < node->key)
        {
            if(cur->right == node)
                return(cur);                        /* Knoten rechts gefunden */
            cur = cur->right;
        }
        else                                         /* Knoten nicht gefunden */
            return(NULL);
    }
}

/*! \fn node_t *tr_find_node(node_t *tree, int key)
    \brief Suche nach einem Knoten mit dem Key key und liefert einen Pointer auf ihn

    \param tree  erstes Element des Baums
    \param key   Key, der gefunden werden soll
    \return pointer to node with Key key (NULL wenn nicht vorhanden)
*/
node_t *tr_find_node(node_t *tree, int key)
{
    node_t *cur = tree;
    if(cur == NULL)
        return(NULL);
    while(1)
        if(cur->key > key)
        {
            if(cur->left == NULL)
                return(NULL);                  /* Key existiert nicht im Baum */
            cur = cur->left;
        }
        else if(cur->key < key)
        {
            if(cur->right == NULL)
                return(NULL);                  /* Key existiert nicht im Baum */
            cur = cur->right;
        }
        else                           /* Keys gleich groß -> Knoten gefunden */
            return(cur);
}

/*! \fn void clear_tree(node_t *tree)
    \brief Löscht den ganzen Baum (zB. Um am Ende den Speicher frei zu geben)

    \param tree erstes Element des Baums
*/
void clear_tree(node_t *tree)
{
    if(tree != NULL)
    {
        clear_tree(tree->left);
        clear_tree(tree->right);

        tree->left = NULL;
        tree->right = NULL;

        tr_delete_node(tree);
    }
}

/*! \fn node_t *tr_create_node(int key)
    \brief Erstellt einen neuen Knoten im Baum. Aus Kapitel 'Trees'
*/
node_t *tr_create_node(int key)
{
    node_t *node;

    node = (node_t *) malloc (sizeof(node_t));
    node->key = key;
    node->left = NULL;
    node->right = NULL;

    return(node);
}

/*! \fn int tr_delete_node(node_t *node)
    \brief Löscht ein node aus dem Baum. Aus Kapitel 'Trees'
*/
int tr_delete_node(node_t *node)
{
    if(node == NULL)
    {
        fprintf(stderr, "kann nicht 'NULL' Löschen\n");
        return(1);
    }
    else if( (node->left != NULL) || (node->right != NULL))
    {
        fprintf(stderr, "kann keinen Knoten mit Kindern löschen!\n");
        return(1);
    }
    free(node);
    return(0);
}

/*! \fn node_t *tr_insert_node(node_t *root, node_t *node, int *in_p)
    \brief Fügt ein node in einen binären Suchbaum root ein. Aus Kapitel 'Trees'

    \param root  Wurzel des binären Suchbaums
    \param node  einzufügender Knoten
    \param in_p  Ist der Key schon vorhanden?
    \return Wurzel des Binären Suchbaums
*/
node_t *tr_insert_node(node_t *root, node_t *node, int *in_p)
{
    node_t *cur = root;

    if(root == NULL)
        return(node);   /* Wenn baum nicht vorhanden, erstellen einen neuen mit
                            der Wurzel node */

    while(1)
        if(cur->key > node->key)
        {
            if(cur->left == NULL)
            {
                cur->left = node;
                break;                     /* Position gefunden, beende Suche */
            }
            cur = cur->left;
        }
        else if(cur->key < node->key)
        {
            if(cur->right == NULL)
            {
                cur->right = node;
                break;                     /* Position gefunden, beende Suche */
            }
            cur = cur->right;
        }
        else                    /* Keys gleich groß -> Knoten schon vorhanden */
        {
            *in_p = 1;
            break;
        }
    return(root);
}

/*! \fn void print_tree(node_t *tree)
    \brief Print tree "inorder"

    \param tree Baum
*/
void print_tree(node_t *tree)
{
    if(tree != NULL)
    {
        print_tree(tree->left);
        printf("%d ", tree->key);
        print_tree(tree->right);
    }
}
