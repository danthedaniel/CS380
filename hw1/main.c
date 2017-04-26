#include <stdio.h>
#include "board.h"

int main(int argc, char** argv) {
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

    board_print(board);
    board_free(board);

exit:
    return ret;
}
