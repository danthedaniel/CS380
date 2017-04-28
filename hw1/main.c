#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "search.h"
#include "board.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    int ret = 0;
    BOARD_t* board = NULL;

    if (argc <= 1) {
        fprintf(stderr, "No file path provided\n");
        ret = 1;
        goto exit;
    }

    board = board_from_file(argv[1]);

    if (board == NULL) {
        ret = 1;
        goto exit;
    }

    // Run with a few search implementations
    random_walk(board, 128);
    depth_first_search(board, 16);

    board_free(board);

exit:
    return ret;
}
