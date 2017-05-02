#ifndef BOARD_H__
#define BOARD_H__

#include <stdint.h>
#include <stdbool.h>

#define GRID_EMPTY 0
#define GRID_WALL  1
#define GRID_GOAL  -1
#define GRID_UNSET -127

#define GOAL_PIECE 2

typedef enum {
    DIR_UP    = 'U',
    DIR_DOWN  = 'D',
    DIR_LEFT  = 'L',
    DIR_RIGHT = 'R'
} DIRECTION_t;

typedef struct {
    uint8_t width;
    uint8_t height;
    int8_t** grid;
} BOARD_t;

typedef struct {
    uint8_t i;
    uint8_t j;
} POINT_t;

typedef struct {
    POINT_t upper_left;
    POINT_t lower_right;
    int8_t value;
} BLOCK_t;

typedef struct {
    DIRECTION_t dir;
    int8_t piece;
    bool goal;
} STATECHANGE_t;

/**
 * Create a BOARD_t from the contents of a file.
 *
 * char* path: The relative or absolute file path to the board specification.
 */
BOARD_t* board_from_file(const char* path);

/**
 * Initialize an empty BOARD_t on the heap.
 */
BOARD_t* board_init();

/**
 * Modifies a board so that its pieces are numbered in a normalized format.
 *
 * BOARD_t* board: The target board.
 */
void board_normalize(BOARD_t* board);

/**
 * Swap all instances of idx1 with idx2, and idx2 with idx1 on a board's grid.
 *
 * BOARD_t* board: The board with the targeted grid.
 * int8_t idx1: The first index to be swapped.
 * int8_t idx2: The second index to be swapped.
 */
void board_grid_swap(BOARD_t* board, int8_t idx1, int8_t idx2);

/**
 * Return a NULL-terminated list of all valid state changes on a board.
 *
 * BOARD_t* board: The target board.
 */
STATECHANGE_t* board_valid_moves(BOARD_t* board);

/**
 * Return the number of unique tiles on the board.
 *
 * BOARD_t* board: The target board.
 */
uint8_t board_num_tiles(BOARD_t* board);

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
 * Compare two boards.
 *
 * BOARD_t* board1: The first board.
 * BOARD_t* board2: The second board.
 */
bool board_equals(BOARD_t* board1, BOARD_t* board2);

/**
 * Check if a board has been solved.
 *
 * BOARD_t* board: The board to check.
 */
bool board_solved(BOARD_t* board);

/**
 * Apply a state change to a board. Returns whether the change resulted in the
 * goal condition being met.
 *
 * BOARD_t* board: The target board.
 * STATECHANGE_t* change: The change specified.
 */
bool board_apply_statechange(BOARD_t* board, BLOCK_t block, STATECHANGE_t* change);

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
 * Expand a set of coordinates of a block to discover the full block, then
 * return a BLOCK_t.
 *
 * BOARD_t* board: The board containing the block.
 * uint8_t i: An i coordinate contained in a block.
 * uint8_t j: A j coordinate contained in a block.
 */
BLOCK_t block_from_coords(BOARD_t* board, uint8_t i, uint8_t j);


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
 * Print a STATECHANGE_t
 */
void statechange_print(STATECHANGE_t* statechange);


/**
 * Parse a signed integer from a null-terminated string.
 *
 * char* int_str: The string to be parsed
 */
int8_t int_parse(char* int_str);

#endif
