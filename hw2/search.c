#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "board.h"
#include "tree.h"
#include "search.h"

// Shared variables for stats
uint32_t NODE_COUNT = 0;
struct timeval t0, tf;

void a_star(BOARD_t* board, uint16_t (*heuristic)(BOARD_t* state)) {
    BOARD_t* board_clone = board_copy(board);
    TREE_t* tree = tree_init(board_clone, NULL);
    LIST_t* open_nodes = list_init(tree);
    LIST_t* closed_nodes = NULL;

    // Reset the stats
    NODE_COUNT = 0;
    gettimeofday(&t0, NULL);

    while (open_nodes != NULL) {
        NODE_COUNT++;
        TREE_t* current = best_node_available(open_nodes, heuristic);

        // Check for win condition
        if (current->change != NULL && current->change->goal) {
            gettimeofday(&tf, NULL);
            print_results(current);

            return;
        }

        open_nodes = list_remove(open_nodes, current);

        if (closed_nodes == NULL) {
            closed_nodes = list_init(current);
        } else {
            BOARD_t* current_clone = board_copy(current->state);
            board_normalize(current_clone);
            TREE_t* normalized_node = tree_init(current_clone, NULL);
            list_append(closed_nodes, normalized_node);
        }

        tree_find_valid_moves(current);
        LIST_t* child = current->children;

        // Check each child
        while (child != NULL) {
            if (list_contains_state(closed_nodes, child->node->state)) {
                tree_free(child->node);
                child->node = NULL;
            } else if (open_nodes == NULL) {
                open_nodes = list_init(child->node);
            } else {
                list_append(open_nodes, child->node);
            }

            child = child->next;
        }
    }

    tree_free(tree);
}

TREE_t* best_node_available(LIST_t* list, uint16_t (*heuristic)(BOARD_t* state)) {
    TREE_t* best = NULL;
    uint16_t min = 65535; // 16bit max value

    // Find the node with the minimum value
    while (list != NULL) {
        uint16_t heuristic_value = heuristic(list->node->state);

        if (heuristic_value < min) {
            min = heuristic_value;
            best = list->node;
        }

        list = list->next;
    }

    return best;
}

uint16_t manhattan_distance(BOARD_t* state) {
    POINT_t goal_corner = board_find_goal_piece(state).upper_left;
    POINT_t goal_pos = board_find_goal(state);

    return abs((int16_t) goal_corner.i - (int16_t) goal_pos.i) +
           abs((int16_t) goal_corner.j - (int16_t) goal_pos.j);
}

uint16_t custom_heuristic(BOARD_t* state) {
    POINT_t goal_corner = board_find_goal_piece(state).upper_left;
    POINT_t goal_pos = board_find_goal(state);

    // Use manhattan distance as a starting point
    uint16_t dist = abs((int16_t) goal_corner.i - (int16_t) goal_pos.i) +
                    abs((int16_t) goal_corner.j - (int16_t) goal_pos.j);

    uint8_t i_min = MIN(goal_corner.i, goal_pos.i);
    uint8_t i_max = MAX(goal_corner.i, goal_pos.i);
    uint8_t j_min = MIN(goal_corner.j, goal_pos.j);
    uint8_t j_max = MAX(goal_corner.j, goal_pos.j);

    // For each empty spot on the path to the goal, subtract one from the
    // distance.
    for (uint8_t i = i_min; i < i_max; i++)
        if (state->grid[i][j_min] == GRID_EMPTY)
            dist = (dist == 0) ? dist : dist - 1;

    for (uint8_t j = j_min; j < j_max; j++)
        if (state->grid[i_max][j] == GRID_EMPTY)
            dist = (dist == 0) ? dist : dist - 1;

    return dist;
}

void solution_print(TREE_t* tree) {
    if (tree->parent != NULL)
        solution_print(tree->parent);

    if (tree->change != NULL)
        statechange_print(tree->change);
}

void print_results(TREE_t* tree) {
    solution_print(tree);
    board_print(tree->state);

    float delta_s = (tf.tv_sec + (tf.tv_usec / 1000000.0)) - (t0.tv_sec + (t0.tv_usec / 1000000.0));
    printf("%u %.2f %d\n\n", NODE_COUNT, delta_s, tree_depth(tree));
}
