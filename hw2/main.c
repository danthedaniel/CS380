#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "search.h"
#include "board.h"

void test_manhattan_distance();
void test_custom_heuristic();

int main(int argc, char** argv) {
    srand(time(NULL));

    test_manhattan_distance();
    test_custom_heuristic();

    return 0;
}

void test_manhattan_distance() {
    BOARD_t* board1 = board_from_file("SBP-level1.txt");

    if (board1 == NULL)
        return;

    printf("Testing A* with manhattan distance on level1 ...\n");
    a_star(board1, manhattan_distance);
    board_free(board1);
}

void test_custom_heuristic() {
    BOARD_t* board1 = board_from_file("SBP-level1.txt");

    if (board1 == NULL)
        return;

    printf("Testing A* with custom heuristic on level1 ...\n");
    a_star(board1, custom_heuristic);
    board_free(board1);
}
