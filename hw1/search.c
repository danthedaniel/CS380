#include <stdint.h>
#include <stdio.h>
#include "board.h"
#include "tree.h"
#include "search.h"

void random_walk(BOARD_t* board, uint32_t N) {
    BOARD_t* board_clone = board_copy(board);
    TREE_t* tree = tree_init(board_clone, NULL);
    // TREE_t* root = tree;

    for (uint32_t i = 0; i < N; ++i) {
        tree_find_valid_moves(tree);
        uint8_t rand_child = rand() % tree->num_children;
        LIST_t* child = tree->children;

        for (uint8_t j = 0; j < rand_child; ++j)
            child = child->next;

        board_print(child->node->state);

        if (child->node->change->goal) {
            printf("Goal state found!");
            break;
        } else {
            tree = child->node;
        }
    }

    tree_free(tree);
}
