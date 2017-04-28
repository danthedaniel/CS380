#ifndef SEARCH_H__
#define SEARCH_H__

#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "tree.h"

/**
 * Solve a board with random walk. Use up to N steps.
 *
 * BOARD_t* board: The board to be solved.
 * uint32_t N: The maximum number of steps to take in solving the board.
 */
void random_walk(BOARD_t* board, uint32_t N);

/**
 * Solve a board with depth-first search. Only travel up to L levels deep.
 *
 * BOARD_t* board: The board to be solved.
 * uint32_t L: The maximum depth.
 */
void depth_first_search(BOARD_t* board, uint32_t L);

/**
 */
bool depth_first_search_iter(TREE_t* tree, uint32_t L);

/**
 * Solve a board with breadth-first search. Only travel up to L levels deep.
 *
 * BOARD_t* board: The board to be solved.
 * uint32_t L: The maximum depth.
 */
void breadth_first_search(BOARD_t* board, uint32_t L);

/**
 * Solve a board with iterative-deepening search. Start with a max-depth of Li,
 * and stop searching at Lm levels deep.
 *
 * BOARD_t* board: The board to be solved.
 * uint32_t Li: Starting depth.
 * uint32_t Lm: Maximum depth.
 */
void iterative_deepening_search(BOARD_t* board, uint32_t Li, uint32_t Lm);

#endif
