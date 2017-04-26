#ifndef TREE_H__
#define TREE_H__

#include <stdint.h>
#include "board.h"

typedef struct TREE_t TREE_t;
typedef struct LIST_t LIST_t;

struct TREE_t {
    BOARD_t* state;
    LIST_t* children;
    uint8_t num_children;
};

/**
 * Initialize a new TREE_t on the heap.
 *
 * BOARD_t* board: The new root for the tree.
 */
TREE_t* tree_init(BOARD_t* board);

/**
 * Append a board state to the end of a tree node's list of children.
 *
 * TREE_t* tree: The tree node that is adding a child.
 * BOARD_t* board: The board state to add.
 */
void tree_add_child(TREE_t* tree, BOARD_t* board);

/**
 * Free all nodes and all children from the heap.
 *
 * TREE_T* root: The top of the tree to be free'd.
 */
void tree_free(TREE_t* root);


struct LIST_t {
    TREE_t* node;
    LIST_t* next;
};

/**
 * Initalize a new LIST_t* on the heap.
 *
 * TREE_t* node: The first element in the list.
 */
LIST_t* list_init(TREE_t* node);

/**
 * Append a new node to a list.
 *
 * LIST_t* list: The list to append to.
 * TREE_t* node: The node to append.
 */
void list_append(LIST_t* list, TREE_t* node);

/**
 * Free a list and all of its children (recursively) from the heap.
 *
 * LIST_t* list: The target list.
 */
void list_free(LIST_t* list);

#endif
