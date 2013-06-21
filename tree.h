#ifndef _TREE_H
#define _TREE_H

#include <stdio.h>
#include <stdlib.h>

/* Datenstruktur für einen Konten in einem binären Suchbaum */
struct node_struct
{
    int                   key;    /*Der Inhalt*/
    struct node_struct  *left;    /*Pointer zum linken  Nachfolger */
    struct node_struct *right;    /*Pointer zum rechten Nachfolger */
};

typedef struct node_struct node_t;

void print_tree(node_t *tree);
node_t *tr_insert_node(node_t *root, node_t *node, int *in_p);
int tr_delete_node(node_t *node);
node_t *tr_create_node(int key);
void clear_tree(node_t *tree);
node_t *tr_find_node(node_t *tree, int key);
node_t *tr_remove_node(node_t *tree, int key_rem, node_t **node_p);
node_t *tr_find_parent(node_t *tree, node_t *node);

#endif /* _TREE_H */
