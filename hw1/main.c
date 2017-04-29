#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "search.h"
#include "board.h"

void test_random_walk();
void test_depth_first_search();
void test_breadth_first_search();
void test_iterative_deepening_search();

int main(int argc, char** argv) {
    srand(time(NULL));

    // test_random_walk();
    // test_depth_first_search();
    test_breadth_first_search();
    // test_iterative_deepening_search();

    return 0;
}

void test_random_walk() {
    BOARD_t* board = board_from_file("SBP-level0.txt");

    if (board == NULL)
        return;

    random_walk(board, 3);
    board_free(board);
}

void test_depth_first_search() {
    BOARD_t* board = board_from_file("SBP-level1.txt");

    if (board == NULL)
        return;

    printf("Performing depth-first search on the board...\n");
    depth_first_search(board, 16);
    board_free(board);
}

void test_breadth_first_search() {
    BOARD_t* board = board_from_file("SBP-level1.txt");

    if (board == NULL)
        return;

    printf("Performing breadth-first search on the board...\n");
    breadth_first_search(board, 17);
    board_free(board);
}

void test_iterative_deepening_search() {
    BOARD_t* board = board_from_file("SBP-level1.txt");

    if (board == NULL)
        return;

    printf("Performing iterative-deepening search on the board...\n");
    iterative_deepening_search(board, 16, 32);
    board_free(board);
}
