#ifndef BOARD_H__
#define BOARD_H__

#include <stdint.h>

typedef struct {
    uint8_t width;
    uint8_t height;
    int8_t** grid;
} BOARD_t;

BOARD_t* board_from_file(char* path);
BOARD_t* board_init();
void board_from_buffer(BOARD_t* board, uint8_t* buffer, uint32_t size);
int8_t board_get(uint8_t x, uint8_t y);
int8_t int_parse(uint8_t* int_str, uint8_t size);

#endif
