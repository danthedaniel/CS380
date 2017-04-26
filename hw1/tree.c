#include <stdint.h>
#include <stdlib.h>
#include "tree.h"
#include "board.h"

TREE_t* tree_init(BOARD_t* board) {
    TREE_t* tree = (TREE_t*) malloc(sizeof(TREE_t));

    tree->state = board;
    tree->children = NULL;
    tree->num_children = 0;

    return tree;
}

void tree_free(TREE_t* root) {
    free(root->state);

    if (root->children != NULL)
        list_free(root->children);
}

void tree_add_child(TREE_t* tree, BOARD_t* board) {
    TREE_t* node = tree_init(board);
    tree->num_children++;

    if (tree->children == NULL)
        tree->children = list_init(node);
    else
        list_append(tree->children, node);
}

LIST_t* list_init(TREE_t* node) {
    LIST_t* list = (LIST_t*) malloc(sizeof(LIST_t));

    list->node = node;
    list->next = NULL;

    return list;
}

void list_append(LIST_t* list, TREE_t* node) {
    while (list->next != NULL)
        list = list->next;

    list->next = list_init(node);
}

void list_free(LIST_t* list) {
    if (list->next != NULL)
        list_free(list->next);

    tree_free(list->node);
}
