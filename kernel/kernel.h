#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

const int VGA_COLS = 80;
const int VGA_ROWS = 25;

volatile uint16_t* vgaBuffer = (uint16_t*)0xB8000;

int column = 0;
int row = 0;
uint8_t color = 0x0F;

enum colors {
    BLACK = 0x00,
    BLUE = 0x01,
    GREEN = 0x02,
    CYAN = 0x03,
    RED = 0x04,
    MAGENTA = 0x05,
    BROWN = 0x06,
    LIGHT_GREY = 0x07,
    DARK_GREY = 0x08,
    LIGHT_BLUE = 0x09,
    LIGHT_GREEN = 0x0A,
    LIGHT_CYAN = 0x0B,
    LIGHT_RED = 0x0C,
    LIGHT_MAGENTA = 0x0D,
    LIGHT_BROWN = 0x0E,
    WHITE = 0x0F,
};

#endif