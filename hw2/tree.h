#ifndef TREE_H__
#define TREE_H__

#include <stdint.h>
#include <stdbool.h>
#include "board.h"

typedef struct TREE_t TREE_t;
typedef struct LIST_t LIST_t;

struct TREE_t {
    BOARD_t* state;
    STATECHANGE_t* change;
    TREE_t* parent;
    LIST_t* children;
    uint8_t num_children;
};

/**
 * Initialize a new TREE_t on the heap.
 *
 * BOARD_t* board: The new root for the tree.
 */
TREE_t* tree_init(BOARD_t* board, STATECHANGE_t* change);

/**
 * Append a board state to the end of a tree node's list of children.
 *
 * TREE_t* tree: The tree node that is adding a child.
 * TREE_t* child: The child node to add.
 */
void tree_add_child(TREE_t* tree, TREE_t* child);

/**
 * Free all nodes and all children from the heap.
 *
 * TREE_T* root: The top of the tree to be free'd.
 */
void tree_free(TREE_t* root);

/**
 * Add all valid moves for all pieces to the current node as children.
 *
 * TREE_t* tree: The currently addressed node in the tree.
 */
void tree_find_valid_moves(TREE_t* tree);

/**
 * Add all valid moves for a specified block on a board grid as children.
 *
 * TREE_t* tree: The current tree node.
 * BLOCK_t block: The block to move.
 */
void tree_add_moves_for_block(TREE_t* tree, BLOCK_t block);

/**
 * Check if a block change is valid, and add it as a child if it is.
 *
 * TREE_t* tree: The tree to add valid moves to.
 * BLOCK_t block: The block to check a move for.
 * DIRECTION_t dir: The direction to check.
 */
void tree_check_block_dir(TREE_t* tree, BLOCK_t block, DIRECTION_t dir);

/**
 * Create a new tree node from a block and a new direction to move it in.
 *
 * BOARD_t* board: The original board (will not mutate).
 * BLOCK_t block: The block to move.
 * DIRECTION_t dir: The direction to move the block in.
 */
TREE_t* tree_from_board_change(BOARD_t* board, BLOCK_t block, DIRECTION_t dir);

/**
 * Determine if a board state, when normalized, matches any normalized board
 * states in a tree's lineage.
 *
 * TREE_t* tree: The tree whos lineage will be tested.
 * BOARD_t* board: The board to compare against (will be normalized).
 */
bool tree_ancestory_contains_state(TREE_t* tree, BOARD_t* board);

/**
 * Return the depth of a node in a tree.
 *
 * TREE_t* tree: A node to test for depth.
 */
uint32_t tree_depth(TREE_t* tree);


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
 * Remove a node from a list by reference. May return NULL (an empty list).
 *
 * LIST_t* list: The list to modify.
 * TREE_t* node: The node to remove.
 */
LIST_t* list_remove(LIST_t* list, TREE_t* node);

/**
 * Whether a list contains a tree with a board state (once normalized).
 */
bool list_contains_state(LIST_t* list, BOARD_t* board);

/**
 * Return true if a list element has a statechange for the given block.
 *
 * LIST_t* list: The list to search.
 * int8_t block: The block index to search for.
 */
bool list_contains_block(LIST_t* list, int8_t block);

/**
 * Free a list and all of its children (recursively) from the heap.
 *
 * LIST_t* list: The target list.
 */
void list_free(LIST_t* list);

#endif
