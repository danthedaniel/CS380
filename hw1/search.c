#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "tree.h"
#include "search.h"

void random_walk(BOARD_t* board, uint32_t N) {
    BOARD_t* board_clone = board_copy(board);
    TREE_t* tree = tree_init(board_clone, NULL);
    TREE_t* root = tree;

    for (uint32_t i = 0; i < N; ++i) {
        tree_find_valid_moves(tree);

        if (tree->num_children == 0) {
            printf("Not sure how, but you've ran out of options...");
            goto free_tree;
        }

        // Now select a random child state
        uint8_t rand_child = rand() % tree->num_children;
        LIST_t* child = tree->children;

        for (uint8_t j = 0; j < rand_child; ++j)
            child = child->next;

        statechange_print(child->node->change);
        printf("\n");
        board_print(child->node->state);
        printf("\n");

        if (child->node->change->goal) {
            printf("Goal state found in %d steps!\n", (i + 1));
            break;
        } else {
            // Expand on selected child
            tree = child->node;
        }
    }

free_tree:
    tree_free(root);
}

void depth_first_search(BOARD_t* board, uint32_t L) {
    BOARD_t* board_clone = board_copy(board);
    TREE_t* tree = tree_init(board_clone, NULL);

    if (!depth_first_search_iter(tree, L))
        printf("No goal found within %d depth\n", L);

    tree_free(tree);
    board_free(board_clone);
}

bool depth_first_search_iter(TREE_t* tree, uint32_t L) {
    if (L < 1)
        return false;

    tree_find_valid_moves(tree);
    LIST_t* child = tree->children;

    while (child != NULL) {
        statechange_print(child->node->change);
        printf("\n");
        board_print(child->node->state);
        printf("\n");

        if (child->node->change->goal) {
            printf("Goal state found!\n");
            return true;
        } else if (tree_ancestory_contains_state(child->node, child->node->state)) {
            tree_free(child->node);
            child->node = NULL;
        } else {
            bool goal_found = depth_first_search_iter(child->node, L - 1);
            if (goal_found)
                return true;
        }

        child = child->next;
    }

    return false;
}

void breadth_first_search(BOARD_t* board, uint32_t L) {
    BOARD_t* board_clone = board_copy(board);
    TREE_t* tree = tree_init(board_clone, NULL);
}

void iterative_deepening_search(BOARD_t* board, uint32_t Li, uint32_t Lm) {
    BOARD_t* board_clone = board_copy(board);
    TREE_t* tree = tree_init(board_clone, NULL);
}
