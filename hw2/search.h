#ifndef SEARCH_H__
#define SEARCH_H__

#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "tree.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

/**
 * Solve a board problem with the A* algorithm.
 *
 * BOARD_t* board: The board to solve.
 * function heuristic: A heuristic function.
 */
void a_star(BOARD_t* board, uint16_t (*heuristic)(BOARD_t* state));

/**
 * Given a list of nodes, return the node in the list with the best calculated
 * heuristic value.
 *
 * LIST_t* list: The list to search.
 * function heuristic: The heuristic to calculate for each node.
 */
TREE_t* best_node_available(LIST_t* list, uint16_t (*heuristic)(BOARD_t* state));


/**
 * A* heuristic that calculates the manhattan distance of the goal block to the
 * goal.
 *
 * BOARD_t* state: That state to calculate the heuristic for.
 */
uint16_t manhattan_distance(BOARD_t* state);

/**
 * A* custom heuristic implementation.
 *
 * BOARD_t* state: That state to calculate the heuristic for.
 */
uint16_t custom_heuristic(BOARD_t* state);


/**
 * Display the solution to a completed board in order.
 *
 * TREE_t* tree: A node containing a solved board state.
 */
void solution_print(TREE_t* tree);

/**
 * Print the solution, ending state, and statistics for a solved board tree.
 *
 * TREE_t* tree: A node containing a solved board state.
 */
void print_results(TREE_t* tree);

#endif
