#ifndef SEARCH_H__
#define SEARCH_H__

#include <stdint.h>
#include "board.h"

/**
 * Solve a board with random walk. Use up to N steps.
 *
 * BOARD_t* board: The board to be solved.
 * uint32_t N: The maximum number of steps to take in solving the board.
 */
void random_walk(BOARD_t* board, uint32_t N);

#endif
