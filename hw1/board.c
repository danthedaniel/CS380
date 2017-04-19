#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "board.h"

BOARD_t* board_init() {
    BOARD_t* board = (BOARD_t*) malloc(sizeof(BOARD_t));

    board->width = 0;
    board->height = 0;
    board->grid = NULL;

    return board;
}

BOARD_t* board_from_file(char* path) {
    BOARD_t* board = NULL;
    uint8_t *buffer = NULL;
    uint32_t file_size = 0;
    FILE* board_file = fopen(path, "r");

    if (board_file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        goto cleanup_fhandler;
    }

    fseek(board_file, 0, SEEK_END);
    file_size = ftell(board_file);
    rewind(board_file);
    buffer = (uint8_t*) malloc(file_size * sizeof(char));
    fread(buffer, file_size, 1, board_file);

    board_from_buffer(board, buffer, file_size);

    if (board == NULL)
        goto cleanup_filebuffer;

cleanup_filebuffer:
    free(buffer);
cleanup_fhandler:
    fclose(board_file);

    return board;
}

void board_from_buffer(BOARD_t* board, uint8_t* buffer, uint32_t size) {
    board = board_init();
    bool header = true;
    uint8_t* start = buffer;
    uint8_t substr_size = 0;
    int8_t dimension = 0;

    while (header) {
        switch (start[substr_size]) {
        case ',':
            substr_size++;
            dimension = int_parse(start, substr_size);

            if (board->width == 0)
                board->width = dimension;
            else if (board->height == 0)
                board->height = dimension;
            break;
        case '\r':
            substr_size++;
            break;
        case '\n':
            header = false;
            break;
        default:
            substr_size++;
            break;
        }
    }

    if (board->width == 0 || board->height == 0)
        goto free_board;



free_board:
    free(board);
    board = NULL;
}

int8_t int_parse(uint8_t* int_str, uint8_t size) {
    int8_t ret = 0;
    bool sign = false;

    for (uint8_t i = 0; i < size; ++i) {
        if (int_str[i] == '-') {
            sign = true;
        }
        else if (int_str[i] >= '0' || int_str[i] <= '9') {
            ret = (ret * 10) + (int_str[i] - '0');
        }
    }

    return sign ? -ret : ret;
}
