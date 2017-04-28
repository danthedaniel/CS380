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
    tree->children = NULL;
    tree->num_children = 0;

    return tree;
}

void tree_free(TREE_t* tree) {
    free(tree->state);
    free(tree->change);

    if (tree->children != NULL)
        list_free(tree->children);
}

void tree_add_child(TREE_t* tree, TREE_t* child) {
    tree->num_children++;

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
    BOARD_t* board = tree->state;
    uint8_t i_min = block.upper_left.i;
    uint8_t i_max = block.lower_right.i;
    uint8_t j_min = block.upper_left.j;
    uint8_t j_max = block.lower_right.j;
    int8_t value = block.value;

    // Check UP
    bool up = true;
    for (uint8_t x = j_min; x <= j_max; x++) {
        int8_t adjacent = board->grid[i_min - 1][x];

        // The move is valid if the adjacent spot is either empty, or we're
        // controlling the goal piece and the adjacent spot is the goal.
        if (value == GOAL_PIECE && adjacent == GRID_GOAL)
            printf("Goal move - value: %d, adjacent: %d\n", value, adjacent);
        else if (adjacent != GRID_EMPTY)
            up = false;
    }

    if (up) {
        TREE_t* child = tree_from_board_change(board, block, DIR_UP);
        tree_add_child(tree, child);
    }

    // Check DOWN
    bool down = true;
    for (uint8_t x = j_min; x <= j_max; x++) {
        int8_t adjacent = board->grid[i_max + 1][x];

        if (value == GOAL_PIECE && adjacent == GRID_GOAL)
            printf("Goal move - value: %d, adjacent: %d\n", value, adjacent);
        else if (adjacent != GRID_EMPTY)
            down = false;
    }

    if (down) {
        TREE_t* child = tree_from_board_change(board, block, DIR_DOWN);
        tree_add_child(tree, child);
    }

    // Check LEFT
    bool left = true;
    for (uint8_t y = i_min; y <= i_max; y++) {
        int8_t adjacent = board->grid[y][j_min - 1];

        if (value == GOAL_PIECE && adjacent == GRID_GOAL)
            printf("Goal move - value: %d, adjacent: %d\n", value, adjacent);
        else if (adjacent != GRID_EMPTY)
            left = false;
    }

    if (left) {
        TREE_t* child = tree_from_board_change(board, block, DIR_LEFT);
        tree_add_child(tree, child);
    }

    // Check RIGHT
    bool right = true;
    for (uint8_t y = i_min; y <= i_max; y++) {
        int8_t adjacent = board->grid[y][j_max + 1];

        if (value == GOAL_PIECE && adjacent == GRID_GOAL)
            printf("Goal move - value: %d, adjacent: %d\n", value, adjacent);
        else if (adjacent != GRID_EMPTY)
            right = false;
    }

    if (right) {
        TREE_t* child = tree_from_board_change(board, block, DIR_RIGHT);
        tree_add_child(tree, child);
    }
}

TREE_t* tree_from_board_change(BOARD_t* board, BLOCK_t block, DIRECTION_t dir) {
    TREE_t* tree = (TREE_t*) malloc(sizeof(TREE_t));
    STATECHANGE_t* change = (STATECHANGE_t*) malloc(sizeof(STATECHANGE_t));

    change->dir = dir;
    change->piece = block.value;

    BOARD_t* new_board = board_copy(board);
    bool goal_met = board_apply_statechange(new_board, block, change);

    change->goal = goal_met;
    tree->state = new_board;
    tree->change = change;
    tree->children = NULL;
    tree->num_children = 0;

    return tree;
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
