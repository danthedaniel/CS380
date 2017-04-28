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
        getchar();
        tree_find_valid_moves(tree);

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
            printf("Goal state found!");
            break;
        } else {
            // Expand on selected child
            tree = child->node;
        }
    }

    tree_free(root);
}
