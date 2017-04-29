#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "tree.h"
#include "search.h"

// Shared variables for
uint32_t NODE_COUNT = 0;
struct timeval t0, tf;

void random_walk(BOARD_t* board, uint32_t N) {
    BOARD_t* board_clone = board_copy(board);
    TREE_t* tree = tree_init(board_clone, NULL);
    TREE_t* root = tree;

    board_print(board_clone);
    printf("\n");

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
    NODE_COUNT = 0;
    gettimeofday(&t0, NULL);

    depth_first_search_iter(tree, L);

    tree_free(tree);
}

bool depth_first_search_iter(TREE_t* tree, uint32_t L) {
    if (L < 1)
        return false;

    tree_find_valid_moves(tree);
    LIST_t* child = tree->children;

    while (child != NULL) {
        TREE_t* leaf = child->node;
        NODE_COUNT++;

        if (child->node->change->goal) {
            gettimeofday(&tf, NULL);
            print_results(child->node);

            return true;
        } else if (tree_ancestory_contains_state(leaf, leaf->state)) {
            tree_free(leaf);
            child->node = NULL;
        } else {
            if (depth_first_search_iter(leaf, L - 1)) {
                return true;
            } else {
                tree_free(leaf);
                child->node = NULL;
            }
        }

        child = child->next;
    }

    return false;
}

void breadth_first_search(BOARD_t* board, uint32_t L) {
    BOARD_t* board_clone = board_copy(board);
    TREE_t* tree = tree_init(board_clone, NULL);
    LIST_t* queue = list_init(tree);
    NODE_COUNT = 0;
    gettimeofday(&t0, NULL);

    while (queue != NULL) {
        NODE_COUNT++;

        // Remove the first element from the queue
        LIST_t* old_queue_head = queue;
        TREE_t* leaf = old_queue_head->node;
        queue = queue->next;
        old_queue_head->next = NULL;
        old_queue_head->node = NULL;
        free(old_queue_head);

        board_print(leaf->state);

        if (leaf->change != NULL && leaf->change->goal) {
            gettimeofday(&tf, NULL);
            print_results(leaf);

            return;
        } else {
            tree_find_valid_moves(leaf);
            LIST_t* child = tree->children;

            while (child != NULL) {
                uint32_t depth = tree_depth(child->node);

                if (tree_ancestory_contains_state(child->node, child->node->state) || depth > L) {
                    tree_free(child->node);
                    child->node = NULL;
                } else if (queue == NULL) {
                    queue = list_init(child->node);
                } else {
                    list_append(queue, child->node);
                }

                child = child->next;
            }
        }
    }

    tree_free(tree);
}

void iterative_deepening_search(BOARD_t* board, uint32_t Li, uint32_t Lm) {
    BOARD_t* board_clone = board_copy(board);
    NODE_COUNT = 0;
    gettimeofday(&t0, NULL);

    for (uint32_t i = Li; i <= Lm; ++i) {
        TREE_t* tree = tree_init(board_clone, NULL);
        bool goal_found = depth_first_search_iter(tree, i);
        tree_free(tree);

        if (goal_found)
            break;
    }
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
