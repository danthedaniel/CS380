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

void board_free(BOARD_t* board) {
    if (board->grid != NULL)
        grid_free(board->grid, board->height);

    free(board);
}

BOARD_t* board_copy(BOARD_t* board) {
    BOARD_t* new_board = board_init();

    new_board->width = board->width;
    new_board->height = board->height;
    new_board->grid = grid_init(board->width, board->height);

    for (uint8_t i = 0; i < board->height; ++i)
        for (uint8_t j = 0; j < board->width; ++j)
            new_board->grid[i][j] = board->grid[i][j];

    return new_board;
}

void board_normalize(BOARD_t* board) {

}

BOARD_t* board_from_file(char* path) {
    BOARD_t* board = NULL;
    char* buffer = NULL;
    uint32_t file_size = 0;
    FILE* board_file = fopen(path, "r");

    if (board_file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        goto cleanup_fhandler;
    }

    fseek(board_file, 0, SEEK_END);
    file_size = ftell(board_file);
    rewind(board_file);
    buffer = (char*) malloc(sizeof(char) * (file_size + 1));
    buffer[file_size] = '\0'; // NULL terminate the buffer
    fread(buffer, file_size, 1, board_file);

    board = board_from_buffer(buffer, file_size);

    free(buffer);
cleanup_fhandler:
    fclose(board_file);

    return board;
}

BOARD_t* board_from_buffer(char* buffer, uint32_t size) {
    char tmp_str[50];
    uint32_t substr_start = 0;
    BOARD_t* board = board_init();

    // Parse board header
    for (uint32_t i = 0; i < size; ++i) {
        switch (buffer[i]) {
        case ',':
            memcpy(tmp_str, &buffer[substr_start], (i - substr_start));
            tmp_str[(i - substr_start)] = '\0';

            if (board->width == 0) {
                board->width = int_parse(tmp_str);
            } else if (board->height == 0) {
                board->height = int_parse(tmp_str);
                board->grid = grid_init(board->width, board->height);
            } else {
                board_add_to_grid(board, int_parse(tmp_str));
            }
        case '\n':
            i++;
            substr_start = i;
            break;
        }
    }

    if (board->width < 3 || board->height < 3) {
        board_free(board);
        board = NULL;
    }

    return board;
}

void board_print(BOARD_t* board) {
    printf("Board: (%dx%d)\n", board->width, board->height);

    for (uint8_t i = 0; i < board->height; ++i) {
        for (uint8_t j = 0; j < board->width; ++j) {
            printf("%d,", board->grid[i][j]);
        }
        printf("\n");
    }
}

int8_t int_parse(char* int_str) {
    int8_t ret = 0;
    bool sign = false;

    for (uint8_t i = 0; i < strlen(int_str); ++i) {
        if (int_str[i] == '-') {
            sign = true;
        }
        else if (int_str[i] >= '0' && int_str[i] <= '9') {
            ret = (ret * 10) + (int_str[i] - '0');
        }
    }

    return sign ? -ret : ret;
}

int8_t** grid_init(uint8_t width, uint8_t height) {
    int8_t** grid = (int8_t**) malloc(sizeof(int8_t*) * height);

    for (uint8_t i = 0; i < height; ++i) {
        grid[i] = (int8_t*) malloc(sizeof(int8_t) * width);
        memset(grid[i], GRID_UNSET, width);
    }

    return grid;
}

void board_add_to_grid(BOARD_t* board, int8_t value) {
    for (uint8_t i = 0; i < board->height; ++i) {
        for (uint8_t j = 0; j < board->width; ++j) {
            if (board->grid[i][j] == GRID_UNSET) {
                board->grid[i][j] = value;
                return;
            }
        }
    }
}

void grid_free(int8_t** grid, uint8_t height) {
    for (int i = 0; i < height; ++i)
        free(grid[i]);

    free(grid);
}
