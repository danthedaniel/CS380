#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "tree.h"
#include "board.h"

TREE_t* tree_init(BOARD_t* board, STATECHANGE_t* change) {
    TREE_t* tree = (TREE_t*) malloc(sizeof(TREE_t));

    tree->state = board;
    tree->change = change;
    tree->parent = NULL;
    tree->children = NULL;
    tree->num_children = 0;

    return tree;
}

void tree_free(TREE_t* tree) {
    if (tree == NULL)
        return;

    free(tree->state);
    free(tree->change);

    if (tree->children != NULL)
        list_free(tree->children);
}

void tree_add_child(TREE_t* tree, TREE_t* child) {
    tree->num_children++;
    child->parent = tree;

    if (tree->children == NULL)
        tree->children = list_init(child);
    else
        list_append(tree->children, child);
}

void tree_find_valid_moves(TREE_t* tree) {
    BOARD_t* board = tree->state;

    // Search from 1..(X - 1) to ignore walls
    for (uint8_t i = 1; i < (board->height - 1); ++i) {
        for (uint8_t j = 1; j < (board->width - 1); ++j) {
            int8_t cell = board->grid[i][j];

            if (cell > GRID_WALL && !list_contains_block(tree->children, cell)) {
                BLOCK_t block = block_from_coords(board, i, j);
                tree_add_moves_for_block(tree, block);
            }
        }
    }
}

void tree_add_moves_for_block(TREE_t* tree, BLOCK_t block) {
    tree_check_block_dir(tree, block, DIR_UP);
    tree_check_block_dir(tree, block, DIR_DOWN);
    tree_check_block_dir(tree, block, DIR_LEFT);
    tree_check_block_dir(tree, block, DIR_RIGHT);
}

void tree_check_block_dir(TREE_t* tree, BLOCK_t block, DIRECTION_t dir) {
    BOARD_t* board = tree->state;
    int8_t value = block.value;
    bool can_move = true;

    bool vertical = (dir == DIR_UP || dir == DIR_DOWN);
    uint8_t axis_min = vertical ? block.upper_left.j : block.upper_left.i;
    uint8_t axis_max = vertical ? block.lower_right.j : block.lower_right.i;

    // Here we sweep across an edge of the block and check if each grid position
    // above/below/left/right of the edge is open.
    for (uint8_t x = axis_min; x <= axis_max; x++) {
        int8_t adjacent;

        switch (dir) {
        case DIR_UP:
            adjacent = board->grid[block.upper_left.i - 1][x];
            break;
        case DIR_DOWN:
            adjacent = board->grid[block.lower_right.i + 1][x];
            break;
        case DIR_LEFT:
            adjacent = board->grid[x][block.upper_left.j - 1];
            break;
        case DIR_RIGHT:
            adjacent = board->grid[x][block.lower_right.j + 1];
            break;
        }

        // The move is valid if the adjacent spot is either empty, or we're
        // controlling the goal piece and the adjacent spot is the goal.
        if (!(value == GOAL_PIECE && adjacent == GRID_GOAL) && (adjacent != GRID_EMPTY))
            can_move = false;
    }

    if (can_move) {
        TREE_t* child = tree_from_board_change(board, block, dir);
        tree_add_child(tree, child);
    }
}

TREE_t* tree_from_board_change(BOARD_t* board, BLOCK_t block, DIRECTION_t dir) {
    STATECHANGE_t* change = (STATECHANGE_t*) malloc(sizeof(STATECHANGE_t));
    change->dir = dir;
    change->piece = block.value;

    BOARD_t* new_board = board_copy(board);
    bool goal_met = board_apply_statechange(new_board, block, change);
    change->goal = goal_met;

    return tree_init(new_board, change);;
}

bool tree_ancestory_contains_state(TREE_t* tree, BOARD_t* board) {
    BOARD_t* board_clone = board_copy(board);
    board_normalize(board_clone);
    tree = tree->parent;

    while (tree != NULL) {
        BOARD_t* ancestor_board = board_copy(tree->state);
        board_normalize(ancestor_board);
        bool boards_equal = board_equals(board_clone, ancestor_board);
        board_free(ancestor_board);

        if (boards_equal) {
            board_free(board_clone);
            return true;
        }

        tree = tree->parent;
    }

    board_free(board_clone);

    return false;
}

uint32_t tree_depth(TREE_t* tree) {
    uint32_t length = 0;

    while (tree->parent != NULL) {
        length++;
        tree = tree->parent;
    }

    return length;
}

LIST_t* list_init(TREE_t* tree) {
    LIST_t* list = (LIST_t*) malloc(sizeof(LIST_t));

    list->node = tree;
    list->next = NULL;

    return list;
}

void list_append(LIST_t* list, TREE_t* tree) {
    while (list->next != NULL)
        list = list->next;

    list->next = list_init(tree);
}

bool list_contains_block(LIST_t* list, int8_t block) {
    while (list != NULL) {
        if (list->node->change->piece == block)
            return true;

        list = list->next;
    }

    return false;
}

void list_free(LIST_t* list) {
    if (list->next != NULL)
        list_free(list->next);

    tree_free(list->node);
}
