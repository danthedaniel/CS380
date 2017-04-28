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

bool board_equals(BOARD_t* board1, BOARD_t* board2) {
    if (board1->width != board2->width || board1->height != board2->height)
        return false;

    for (uint8_t i = 0; i < board1->height; ++i) {
        for (uint8_t j = 0; j < board1->width; ++j) {
            if (board1->grid[i][j] != board2->grid[i][j])
                return false;
        }
    }

    return true;
}

bool board_solved(BOARD_t* board) {
    for (uint8_t i = 0; i < board->height; ++i)
        for (uint8_t j = 0; j < board->width; ++j)
            if (board->grid[i][j] == GRID_GOAL)
                return false;

    return true;
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
            // Fall through
        case '\n':
            i++;
            substr_start = i;
            break;
        }
    }

    if (board->width < 3 || board->height < 3) {
        fprintf(stderr, "Board file is invalid\n");
        board_free(board);
        board = NULL;
    }

    return board;
}

void board_print(BOARD_t* board) {
    printf("%d,%d,\n", board->width, board->height);

    for (uint8_t i = 0; i < board->height; ++i) {
        for (uint8_t j = 0; j < board->width; ++j) {
            printf("%d,", board->grid[i][j]);
        }
        printf("\n");
    }
}

BLOCK_t block_from_coords(BOARD_t* board, uint8_t i, uint8_t j) {
    int8_t value = board->grid[i][j];
    uint8_t i_max = i;
    uint8_t i_min = i;
    uint8_t j_max = j;
    uint8_t j_min = j;

    // Find i/j minimums and maximums for the block
    while (board->grid[++i_max][j] == value) {}
    i_max--;

    while (board->grid[--i_min][j] == value) {}
    i_min++;

    while (board->grid[i][++j_max] == value) {}
    j_max--;

    while (board->grid[i][--j_min] == value) {}
    j_min++;

    BLOCK_t block;
    block.upper_left.i = i_min;
    block.upper_left.j = j_min;
    block.lower_right.i = i_max;
    block.lower_right.j = j_max;
    block.value = value;

    return block;
}

bool board_apply_statechange(BOARD_t* board, BLOCK_t block, STATECHANGE_t* change) {
    bool goal_met = false;
    int8_t i_offset = 0;
    int8_t j_offset = 0;

    switch (change->dir) {
    case DIR_UP:
        i_offset = -1;
        break;
    case DIR_DOWN:
        i_offset = +1;
        break;
    case DIR_LEFT:
        j_offset = -1;
        break;
    case DIR_RIGHT:
        j_offset = +1;
        break;
    }

    BLOCK_t new_block;
    new_block.value = block.value;
    new_block.upper_left.i = block.upper_left.i + i_offset;
    new_block.upper_left.j = block.upper_left.j + j_offset;
    new_block.lower_right.i = block.lower_right.i + i_offset;
    new_block.lower_right.j = block.lower_right.j + j_offset;

    // Fill old block with empty spaces
    for (uint8_t i = block.upper_left.i; i <= block.lower_right.i; ++i) {
        for (uint8_t j = block.upper_left.j; j <= block.lower_right.j; ++j) {
            board->grid[i][j] = GRID_EMPTY;
        }
    }

    // Fill new block in
    for (uint8_t i = new_block.upper_left.i; i <= new_block.lower_right.i; ++i) {
        for (uint8_t j = new_block.upper_left.j; j <= new_block.lower_right.j; ++j) {
            if (board->grid[i][j] == GRID_GOAL && new_block.value == GOAL_PIECE)
                goal_met = true;

            board->grid[i][j] = new_block.value;
        }
    }

    return goal_met;
}

void board_normalize(BOARD_t* board) {
    int8_t nextIdx = GOAL_PIECE + 1;

    for (uint8_t i = 0; i < board->height; ++i) {
        for (uint8_t j = 0; j < board->width; ++j) {
            if (board->grid[i][j] == nextIdx) {
                nextIdx++;
            } else if (board->grid[i][j] > nextIdx) {
                board_grid_swap(board, nextIdx, board->grid[i][j]);
                nextIdx++;
            }
        }
    }
}

void board_grid_swap(BOARD_t* board, int8_t idx1, int8_t idx2) {
    for (uint8_t i = 0; i < board->height; ++i) {
        for (uint8_t j = 0; j < board->width; ++j) {
            if (board->grid[i][j] == idx1) {
                board->grid[i][j] = idx2;
            } else if (board->grid[i][j] == idx2) {
                board->grid[i][j] = idx1;
            }
        }
    }
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

void statechange_print(STATECHANGE_t* statechange) {
    printf("(%d,'%c')\n", statechange->piece, statechange->dir);
}
