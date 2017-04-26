#ifndef BOARD_H__
#define BOARD_H__

#include <stdint.h>

#define GRID_UNSET -127

typedef struct {
    uint8_t width;
    uint8_t height;
    int8_t** grid;
} BOARD_t;

/**
 * Create a BOARD_t from the contents of a file.
 *
 * char* path: The relative or absolute file path to the board specification.
 */
BOARD_t* board_from_file(char* path);

/**
 * Initialize an empty BOARD_t on the heap.
 */
BOARD_t* board_init();

/**
 * When parsing the board specification file, will set the first unset cell to
 * the value specified.
 *
 * BOARD_t* board: The target board.
 * int8_t value: The value to set the cell to.
 */
void board_add_to_grid(BOARD_t* board, int8_t value);

/**
 * Copy a board struct into a new board on the heap.
 *
 * BOARD_t* board: The board to be copied from.
 */
BOARD_t* board_copy(BOARD_t* board);

/**
 * Free a board and its grid from the heap.
 *
 * BOARD_t* board: The target board.
 */
void board_free(BOARD_t* board);

/**
 * Parse a file buffer and assign appropriate values to a board structure.
 *
 * char* buffer: File buffer to be parsed. This is mutated in the fuction.
 * uint32_t size: The size of the file buffer.
 */
BOARD_t* board_from_buffer(char* buffer, uint32_t size);

/**
 * Pretty-print a board.
 *
 * BOARD_t* board: The target board.
 */
void board_print(BOARD_t* board);


/**
 * Initialize an empty 2-dimensional array of int8_ts on the heap.
 *
 * uint8_t width: The desired width of the array.
 * uint8_t height: The desired height of the array.
 */
int8_t** grid_init(uint8_t width, uint8_t height);

/**
 * Free a 2-dimentional array from the heap.
 *
 * int8_t** grid: The target array.
 * uint8_t height: The array's height.
 */
void grid_free(int8_t** grid, uint8_t height);


/**
 * Parse a signed integer from a null-terminated string.
 *
 * char* int_str: The string to be parsed
 */
int8_t int_parse(char* int_str);

#endif
